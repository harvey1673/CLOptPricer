#ifndef FXPRICER_H
#define FXPRICER_H
#include <string>
#include "volmodel.h"
#include "euopt.h"
#include "barrier.h"
#include "math_utils.h"
#include "pricer.h"

class FXPricer : public Pricer {
public:
	FXPricer(const double dtoday, const double dexp, 
			const double fwd, FXSamuelVolNode *vol, const double strike, const double ir, 
			std::string otype, DblVector &fxTenors, DblVector& fxFwds);
	~FXPricer() { delete _fwdInterp; }
	double GetFXFwdByDate(const double dexp) { return _fwdInterp->InterpByExpiry(dexp); }
	virtual void setFxFwd(const double fxfwd, unsigned int idx);
	virtual void setFxVol(const double fxvol, unsigned int idx);
	virtual double fxdelta();
	virtual DblVector fxdeltas();
	virtual double fxvega();
	virtual DblVector fxvegas();
	virtual double paydate() { return _dpayment; }
	virtual void setPaydate(const double dpay) { _dpayment = dpay; }
private:
	DblVector& _fxTenors;
	DblVector& _fxFwds;
	FwdInterp* _fwdInterp;
	double _dpayment;
};	

class FXBlackPricer: public FXPricer {
public:
	FXBlackPricer( const double dtoday, const double dexp, 
			const double fwd, FXSamuelVolNode *vol, const double strike, 
			const double ir, std::string otype, 
			DblVector& fxTenors, DblVector& fxFwds )
			: FXPricer( dtoday, dexp, fwd, vol, strike, ir, otype, fxTenors, fxFwds) {}
	virtual double price();
};

class FXDigitalPricer: public FXPricer {
public:
	FXDigitalPricer( const double dtoday, const double dexp, 
			const double fwd, FXSamuelVolNode *vol, const double strike, 
			const double ir, std::string otype, 
			DblVector& fxTenors, DblVector& fxFwds )
			: FXPricer( dtoday, dexp, fwd, vol, strike, ir, otype, fxTenors, fxFwds) {}
	virtual double price();
};

template <typename T>
class FXStripPricer : public FXPricer {
public:
	FXStripPricer( const double dtoday, 
		const double startDate, const double endDate, 
		const double fwd, FXSamuelVolNode *vol,
		const double strike, const double ir, 
		const std::string otype, const DblVector &hols, 
		DblVector& fxTenors, DblVector& fxFwds );
	~FXStripPricer();
	virtual double price();
	virtual void setFwd( const double fwd);
	virtual void setStrike( const double strike);
	virtual void setVol( VolNode *vol);
	virtual void setIR( const double ir);
	virtual void setToday( const double dtoday);
	virtual void setPaydate( const double dpay);
private:
	vector<T *> _pvec;
	DblVector _bdays;
	DblVector _hols;
	double _sDate;
	double _eDate;
};

typedef FXStripPricer<FXBlackPricer> FXBlackStripPricer;
typedef FXStripPricer<FXDigitalPricer>  FXBinStripPricer;

#endif
