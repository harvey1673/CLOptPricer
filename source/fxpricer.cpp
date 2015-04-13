#include "fxpricer.h"

FXPricer::FXPricer(const double dtoday, const double dexp, 
			const double fwd, VolNode *vol, const double strike, const double ir, 
			std::string otype, DblVector fxTenors, DblVector fxFwds) :
			Pricer(dtoday, dexp, fwd, vol, strike, ir, otype), _fxTenors(fxTenors), _fxFwds(fxFwds)
{
	_fwdInterp = new FwdInterp(dtoday, &_fxTenors, &_fxFwds);
}

double FXPricer::fxdelta()
{
	DblVector fxfwd;;
	double eps = this->priceTweak();
	double spot = fxfwd[0];
	for (size_t i = 0; i < _fxFwds.size(); ++i)
	{
		fxfwd.push_back(_fxFwds[i]);
		_fxFwds[i] = _fxFwds[i] * ( 1 + eps );
	}
	double uprice = this->price();
	for (size_t i = 0; i < _fxFwds.size(); ++i)
		_fxFwds[i] = fxfwd[i] * ( 1 - eps );
	double dprice = this->price();
	for (size_t i = 0; i < _fxFwds.size(); ++i)
		_fxFwds[i] = fxfwd[i];
	return (uprice - dprice)/(2*eps*spot);
}

DblVector FXPricer::fxdeltas()
{
	DblVector deltas;
	double fxfwd;
	double eps = this->priceTweak();
	for (size_t i = 0; i < _fxFwds.size(); ++i)
	{
		fxfwd = _fxFwds[i];
		_fxFwds[i] = fxfwd * ( 1 + eps );
		double uprice = this->price();
		_fxFwds[i] = fxfwd * ( 1 - eps );
		double dprice = this->price();
		_fxFwds[i] = fxfwd;
		deltas.push_back((uprice - dprice)/(2*eps*fxfwd));
	}
	return deltas;
}

double FXPricer::fxvega()
{
	VolNode* vol = this->vol_();
	double dvol = vol->fxAtmVol(0) * this->volTweak();
	DblVector fxvols;
	for (size_t i = 0; i < vol->fxAtmVols_().size(); ++i)
	{
		double v = vol->fxAtmVol(i)
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
	VolNode* vol = this->vol_();
	double eps = this->volTweak();
	for (size_t i = 0; i < vol->fxAtmVols_().size(); ++i)
	{
		double v = vol->fxAtmVol(i)
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
	VolNode *vol = this->vol_();
	double fwd = this->fwd_();
	double dtoday = this->dtoday_();
	double dexp = this->dexp_();
	double strike = this->strike_();
	double fxfwd = GetFXFwdByDate(dexp);
	double comfwd = fwd * fxfwd;
	double comvol = vol->GetVolByMoneyness( std::log(strike/fwd), dexp)
	double tExp = (dexp - dtoday)/365.0;
	double df = std::exp(-this->irate_()*tExp);
	std::string PutCall = this->otype_();
	double pr = BlackPrice(comfwd, strike, comvol, tExp, 1, PutCall);
	pr = pr/fxfwd * df;
	return pr;
}

double FXDigitalPricer::price()
{
	VolNode *vol = this->vol_();
	double fwd = this->fwd_();
	double dtoday = this->dtoday_();
	double dexp = this->dexp_();
	double strike = this->strike_();
	double fxfwd = GetFXFwdByDate(dexp);
	double comfwd = fwd * fxfwd;
	double tExp = (dexp - dtoday)/365.0;
	double df = std::exp(-this->irate_()*tExp);
	std::string otype = this->otype_();
	DigitalPricer dp(dtoday, dexp, comfwd, vol, strike, 0.0, otype);
	double pr = dp.price();
	pr = pr/fxfwd * df;
	return pr;
}

