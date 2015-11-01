#include "fxpricer.h"

FXPricer::FXPricer(const double dtoday, const double dexp, 
			const double fwd, FXSamuelVolNode *vol, const double strike, const double ir, 
			std::string otype, DblVector& fxTenors, DblVector& fxFwds) :
			Pricer(dtoday, dexp, fwd, vol, strike, ir, otype), _fxTenors(fxTenors), _fxFwds(fxFwds)
{
	_dpayment = dexp;
	_fwdInterp = new FwdInterp(dtoday, &_fxTenors, &_fxFwds);
}

void FXPricer::setFxFwd(const double fxfwd, unsigned int idx)
{
	if (idx < _fxFwds.size())
		_fxFwds[idx] = fxfwd;
}

void FXPricer::setFxVol(const double fxvol, unsigned int idx)
{
	FXSamuelVolNode* vol = static_cast<FXSamuelVolNode*>(this->vol_());
	if (idx < vol->fxAtmVols_().size())
		vol->setFxAtmVols(fxvol, idx);
}

double FXPricer::fxdelta()
{
	DblVector fxfwd;;
	double eps = this->priceTweak();
	double spot = _fxFwds[0];
	for (size_t i = 0; i < _fxFwds.size(); ++i)
	{
		fxfwd.push_back(_fxFwds[i]);
		this->setFxFwd(_fxFwds[i] /( 1 + eps ), i );
	}
	double uprice = this->price();
	for (size_t i = 0; i < _fxFwds.size(); ++i)
		this->setFxFwd( fxfwd[i]  / ( 1 - eps ), i );
	double dprice = this->price();
	for (size_t i = 0; i < _fxFwds.size(); ++i)
		this->setFxFwd( fxfwd[i], i );
	return (uprice - dprice)/(2*eps/spot);
}

DblVector FXPricer::fxdeltas()
{
	DblVector deltas;
	double fxfwd;
	double eps = this->priceTweak();
	for (size_t i = 0; i < _fxFwds.size(); ++i)
	{
		fxfwd = _fxFwds[i];
		this->setFxFwd( fxfwd / ( 1 + eps ), i );
		double uprice = this->price();
		this->setFxFwd( fxfwd / ( 1 - eps ), i );
		double dprice = this->price();
		this->setFxFwd( fxfwd, i );
		deltas.push_back((uprice - dprice)/(2*eps/fxfwd));
	}
	return deltas;
}

double FXPricer::fxvega()
{
	FXSamuelVolNode* vol = static_cast<FXSamuelVolNode*>(this->vol_());
	double dvol = vol->fxAtmVol(0) * this->volTweak();
	DblVector fxvols;
	for (size_t i = 0; i < vol->fxAtmVols_().size(); ++i)
	{
		double v = vol->fxAtmVol(i);
		fxvols.push_back(v);
		vol->setFxAtmVols(v+dvol, i);
	}
	double uprice = this->price();
	for (size_t i = 0; i < vol->fxAtmVols_().size(); ++i)
		vol->setFxAtmVols(fxvols[i]-dvol, i);
	double dprice = this->price();
	for (size_t i = 0; i < vol->fxAtmVols_().size(); ++i)
		vol->setFxAtmVols(fxvols[i], i);
	return (uprice - dprice)/2/dvol;
}

DblVector FXPricer::fxvegas()
{
	DblVector vegas;
	FXSamuelVolNode* vol = static_cast<FXSamuelVolNode*>(this->vol_());
	double eps = this->volTweak();
	DblVector atmVols = vol->fxAtmVols_();
	for (size_t i = 0; i < atmVols.size(); ++i)
	{
		double v = vol->fxAtmVol(i);
		vol->setFxAtmVols(v*(1+eps), i);
		double uprice = this->price();
		vol->setFxAtmVols(v*(1-eps), i);
		double dprice = this->price();
		vegas.push_back((uprice-dprice)/(2*eps*v));
		vol->setFxAtmVols(v, i);
	}
	return vegas;
}

double FXBlackPricer::price()
{
	FXSamuelVolNode *vol = static_cast<FXSamuelVolNode*>(this->vol_());
	double fwd = this->fwd_();
	double dtoday = this->dtoday_();
	double dexp = this->dexp_();
	double strike = this->strike_();
	double fxfwd = this->GetFXFwdByDate(dexp);
	double comfwd = fwd * fxfwd;
	double comvol = vol->GetVolByMoneyness( std::log(strike/fwd), dexp);
	double tExp = (dexp - dtoday)/365.0;
	std::string PutCall = this->otype_();
	double pr = BlackPrice(comfwd, strike, comvol, tExp, 1, PutCall);
	double dpay = this->paydate();
	double tPay = (dpay - dtoday)/365.0;
	double df = std::exp(-this->irate_()*tPay);
	fxfwd = this->GetFXFwdByDate(dpay);
	pr = pr/fxfwd * df;
	return pr;
}

double FXDigitalPricer::price()
{
	FXSamuelVolNode *vol = static_cast<FXSamuelVolNode*>(this->vol_());
	double fwd = this->fwd_();
	double dtoday = this->dtoday_();
	double dexp = this->dexp_();
	double strike = this->strike_();
	double fxfwd = this->GetFXFwdByDate(dexp);
	double comfwd = fwd * fxfwd;
	double tExp = (dexp - dtoday)/365.0;
	std::string otype = this->otype_();
	DigitalPricer dp(dtoday, dexp, comfwd, vol, strike, 0.0, otype);
	double pr = dp.price();
	double dpay = this->paydate();
	double tPay = (dpay - dtoday)/365.0;
	double df = std::exp(-this->irate_()*tPay);
	fxfwd = this->GetFXFwdByDate(dpay);
	pr = pr/fxfwd * df;
	return pr;
}

template <typename T>
FXStripPricer<T>::FXStripPricer( const double dtoday, 
	const double startDate, const double endDate, 
	const double fwd, FXSamuelVolNode *vol,
	const double strike, const double ir, 
	const std::string otype, const DblVector &hols, 
	DblVector& fxTenors, DblVector& fxFwds ):
	FXPricer( dtoday, endDate, fwd, vol, strike, ir, otype, fxTenors, fxFwds), 
	_hols(hols), _sDate(startDate), _eDate(endDate)
{
	_bdays = businessDays(startDate, endDate, hols);
	for (size_t i = 0; i < _bdays.size(); ++i)
	{
		_pvec.push_back( new T(dtoday, _bdays[i], fwd, vol, strike, ir, otype, fxTenors, fxFwds));
	}
	this->setPaydate(endDate);
}

template <typename T>
FXStripPricer<T>::~FXStripPricer()
{
	for (size_t i = 0; i < _pvec.size(); ++i) delete _pvec[i];
	_pvec.clear();
}

template <typename T>
void FXStripPricer<T>::setFwd(const double fwd)
{
	Pricer::setFwd(fwd);
	for (size_t i=0; i< _pvec.size(); ++i ) 
	{
		_pvec[i]->setFwd(fwd);
	}
}

template <typename T>
void FXStripPricer<T>::setStrike(const double strike)
{
	Pricer::setStrike(strike);
	for (size_t i=0; i< _pvec.size(); ++i ) 
	{
		_pvec[i]->setStrike(strike);
	}
}

template <typename T>
void FXStripPricer<T>::setVol(VolNode *vol)
{ 
	Pricer::setVol(vol);
	for (size_t i=0; i< _pvec.size(); ++i ) 
		_pvec[i]->setVol(vol);
}

template <typename T>
void FXStripPricer<T>::setIR(const double ir)
{ 
	FXPricer::setIR(ir);
	for (size_t i=0; i< _pvec.size(); ++i ) 
		_pvec[i]->setIR(ir);
}

template <typename T>
void FXStripPricer<T>::setToday(const double dtoday)
{ 
	FXPricer::setToday(dtoday);
	for (size_t i=0; i< _pvec.size(); ++i ) 
		_pvec[i]->setToday(dtoday);
}

template <typename T>
void FXStripPricer<T>::setPaydate(const double dpay)
{ 
	FXPricer::setPaydate(dpay);
	for (size_t i=0; i< _pvec.size(); ++i ) 
		_pvec[i]->setPaydate(dpay);
}

template <typename T>
double FXStripPricer<T>::price()
{
	double psum = 0.0;
	if (_pvec.size() == 0)
		return 0.0;
	else 
	{
		for (size_t i=0; i< _pvec.size(); ++i )
		{	
			psum += _pvec[i]->price();
		}
		//double dexp = this->dexp_();
		//double dtoday = this->dtoday_();
		//double tExp = (dexp - dtoday)/365.0;
		//double df = std::exp(-this->irate_()*tExp);
		return psum/_pvec.size();
	}
}

template class FXStripPricer<FXBlackPricer>;    
template class FXStripPricer<FXDigitalPricer>;

