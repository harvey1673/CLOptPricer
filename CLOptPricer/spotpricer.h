#ifndef SPOTPRICER_H
#define SPOTPRICER_H

#include <string>
#include "volmodel.h"
#include "euopt.h"
#include "spotbarrier.h"
#include "math_utils.h"

class SpotPricer {
public:
	SpotPricer(const double dtoday, const double dexp,
		const double spot, VolNode *vol,
		const double strike, const double ir, const double div, std::string otype) :
		_dtoday(dtoday), _dexp(dexp),
		_spot(spot), _vol(vol),
		_strike(strike), _irate(ir), _div(div), _otype(otype) {}

	virtual double price() = 0;
	virtual double delta();
	virtual double gamma();
	virtual double vega();
	virtual double theta();

	virtual void setStrike(const double strike) { _strike = strike; }
	virtual void setSpot(const double spot) { _spot = spot; }
	virtual void setVol(VolNode *vol) { _vol = vol; }
	virtual void setIR(const double ir) { _irate = ir; }
	virtual void setDiv(const double div) { _div = div; }
	virtual void setExpiry(const double dexp) { _dexp = dexp; }
	virtual void setToday(const double dtoday) { _dtoday = dtoday; }
	void setOtype(const std::string otype) { _otype = otype; }
	double strike_() { return _strike; }
	double spot_() { return _spot; }
	VolNode* vol_() { return _vol; }
	double dexp_() { return _dexp; }
	double dtoday_() { return _dtoday; }
	double irate_() { return _irate; }
	double div_() { return _div; }
	std::string otype_() { return _otype; }
	virtual double priceTweak() { return 0.001; }
	virtual double volTweak() { return 0.01; }
private:
	double _spot;
	VolNode *_vol;
	double _strike;
	double _dexp;
	double _dtoday;
	double _irate;
	double _div;
	std::string _otype;
};


class SpotBarrierPricer : public SpotPricer {
public:
	SpotBarrierPricer(const double dtoday, const double dexp,
		const double spot, VolNode *vol, const double strike,
		const double barrier, const std::string btype,
		const double ir, const double div, std::string otype, const std::string mtype)
		: SpotPricer(dtoday, dexp, spot, vol, strike, ir, div, otype),
		_barrier(barrier), _btype(btype), _mtype(mtype) {}

	virtual double price();

	void setBarrier(const double barrier) { _barrier = barrier; }
	void setBtype(const std::string btype) { _btype = btype; }
	void setMtype(const std::string mtype) { _mtype = mtype; }

	double barrier_() { return _barrier; }
	std::string btype_()   { return _btype; }
	std::string mtype_()   { return _mtype; }
private:
	double _barrier;
	std::string _btype;
	std::string _mtype;
};

class SpotBarrierStripPricer : public SpotPricer {
public:
	SpotBarrierStripPricer(const double dtoday,
		const double startDate, const double endDate,
		const double spot, VolNode *vol, const double strike,
		const double barrier, const std::string btype,
		const double ir, const double div, std::string otype, const std::string mtype,
		const DblVector &hols);
	virtual double price();
	virtual void setSpot(const double spot);
	virtual void setVol(VolNode *vol);
	virtual void setIR(const double ir);
	virtual void setDiv(const double ir);
	virtual void setToday(const double dtoday);

	void setBarrier(const double barrier) { _barrier = barrier; }
	void setBtype(const std::string btype) { _btype = btype; }
	void setMtype(const std::string mtype) { _mtype = mtype; }

	double barrier_() { return _barrier; }
	std::string btype_()   { return _btype; }
	std::string mtype_()   { return _mtype; }

private:
	vector<SpotBarrierPricer> _pvec;
	DblVector _bdays;
	DblVector _hols;
	double _sDate;
	double _eDate;
	double _barrier;
	std::string _btype;
	std::string _mtype;
};


#endif
