#include "spotpricer.h"

#define PRICE_EPS 0.005
#define VOL_EPS 0.01

double SpotPricer::delta()
{
	double spot = this->spot_();
	double eps = this->priceTweak();
	double up = (1 + eps)*spot;
	this->setSpot(up);
	double uprice = this->price();

	double dp = (1 - eps)*spot;
	this->setSpot(dp);
	double dprice = this->price();

	this->setSpot(spot);

	return (uprice - dprice) / (up - dp);
}

double SpotPricer::gamma()
{
	double spot = this->spot_();
	double price = this->price();
	double eps = this->priceTweak();
	double up = (1 + eps)*spot;
	this->setSpot(up);
	double uprice = this->price();

	double dp = (1 - eps)*spot;
	this->setSpot(dp);
	double dprice = this->price();

	this->setSpot(spot);

	return (uprice + dprice - 2 * price) / (eps*eps*spot*spot);
}

double SpotPricer::vega()
{
	VolNode* vol = this->vol_();
	double atmvol = vol->atmVol_();
	double eps = this->volTweak();

	double uvol = (1 + eps) * atmvol;
	vol->setAtm(uvol);
	double uprice = this->price();

	double dvol = (1 - eps) * atmvol;
	vol->setAtm(dvol);
	double dprice = this->price();

	vol->setAtm(atmvol);

	return (uprice - dprice) / (uvol - dvol);
}

double SpotPricer::theta()
{
	double price = this->price();
	double dtoday = this->dtoday_();
	double dexp = this->dexp_();

	if (dtoday >= dexp)
		return 0.0;

	double dnext = (dexp < dtoday + 1) ? dexp : dtoday + 1;
	this->setToday(dnext);
	double nextprice = this->price();
	this->setToday(dtoday);
	return nextprice - price;
}

double SpotBarrierPricer::price()
{
	VolNode *volnode = this->vol_();
	double spot = this->spot_();
	double strike = this->strike_();
	double barrier = this->barrier_();
	double dtoday = this->dtoday_();
	double dexp = this->dexp_();
	double tExp = volnode->time2expiry_(dtoday, dexp);	
	double ir = this->irate_();
	double div = this->div_();

	std::string otype = this->otype_();
	std::string btype = this->btype_();
	std::string mtype = this->mtype_();

	double vol = volnode->GetVolByMoneyness(0, this->dexp_());

	if (mtype == "d") {
		double discreteAdj = std::exp(0.5826 * vol / std::sqrt(252.0));
		if (spot > barrier)
		{
			barrier = barrier / discreteAdj;
		}
		else if (spot < barrier)
		{
			barrier = barrier * discreteAdj;
		}
	}

	double price = -1;

	if ((btype == "do") || (btype == "DO"))
	{
		if ((otype == "c") || (otype == "C"))
			price = SpotBarrierSingleFull_doc(spot, vol, tExp, strike, barrier, ir, div);
		else
			price = SpotBarrierSingleFull_dop(spot, vol, tExp, strike, barrier, ir, div);
	}
	else if ((btype == "di") || (btype == "DI"))
	{
		if ((otype == "c") || (otype == "C"))
			price = SpotBarrierSingleFull_dic(spot, vol, tExp, strike, barrier, ir, div);
		else
			price = SpotBarrierSingleFull_dip(spot, vol, tExp, strike, barrier, ir, div);
	}
	else if ((btype == "uo") || (btype == "UO"))
	{
		if ((otype == "c") || (otype == "C"))
			price = SpotBarrierSingleFull_uoc(spot, vol, tExp, strike, barrier, ir, div);
		else
			price = SpotBarrierSingleFull_uop(spot, vol, tExp, strike, barrier, ir, div);
	}
	else if ((btype == "ui") || (btype == "UI"))
	{
		if ((otype == "c") || (otype == "C"))
			price = SpotBarrierSingleFull_uic(spot, vol, tExp, strike, barrier, ir, div);
		else
			price = SpotBarrierSingleFull_uip(spot, vol, tExp, strike, barrier, ir, div);
	}

	return price;
}

SpotBarrierStripPricer::SpotBarrierStripPricer(const double dtoday,
	const double startDate, const double endDate,
	const double spot, VolNode *vol, const double strike,
	const double barrier, const std::string btype,
	const double ir, const double div, std::string otype, const std::string mtype,
	const DblVector &hols) :
	SpotPricer(dtoday, endDate, spot, vol, strike, ir, div, otype),
	_barrier(barrier), _btype(btype), _mtype(mtype),
	_hols(hols), _sDate(startDate), _eDate(endDate)
{
	_bdays = businessDays(startDate, endDate, hols);
	for (size_t i = 0; i < _bdays.size(); ++i)
		_pvec.push_back(SpotBarrierPricer(dtoday, _bdays[i], spot, vol, strike, barrier, btype, ir, div, otype, mtype));
}

double SpotBarrierStripPricer::price()
{
	double psum = 0.0;
	if (_pvec.size() == 0)
		return 0.0;
	else {
		for (size_t i = 0; i< _pvec.size(); ++i)
			psum += _pvec[i].price();

		return psum / _pvec.size();
	}
}

void SpotBarrierStripPricer::setSpot(const double spot)
{
	SpotPricer::setSpot(spot);
	for (size_t i = 0; i< _pvec.size(); ++i)
		_pvec[i].setSpot(spot);
}

void SpotBarrierStripPricer::setVol(VolNode *vol)
{
	SpotPricer::setVol(vol);
	for (size_t i = 0; i< _pvec.size(); ++i)
		_pvec[i].setVol(vol);
}

void SpotBarrierStripPricer::setIR(const double ir)
{
	SpotPricer::setIR(ir);
	for (size_t i = 0; i< _pvec.size(); ++i)
		_pvec[i].setIR(ir);
}

void SpotBarrierStripPricer::setDiv(const double div)
{
	SpotPricer::setDiv(div);
	for (size_t i = 0; i< _pvec.size(); ++i)
		_pvec[i].setDiv(div);
}

void SpotBarrierStripPricer::setToday(const double dtoday)
{
	SpotPricer::setToday(dtoday);
	for (size_t i = 0; i< _pvec.size(); ++i)
		_pvec[i].setToday(dtoday);
}