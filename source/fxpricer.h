#ifndef FXPRICER_H
#define FXPRICER_H
#include "pricer.h"

class FXPricer : public Pricer {
public:
	FXPricer(const double dtoday, const double dexp, 
			const double fwd, VolNode *vol, const double strike, const double ir, 
			std::string otype, DblVector fxTenors, DblVector fxFwds);
	double GetFXFwdByDate(const double dexp) { return _fwdInterp->InterpByExpiry(dexp); }
	virtual double fxdelta();
	virtual DblVector fxdeltas();
	virtual double fxvega();
	virtual DblVector fxvegas();
private:
	DblVector _fxTenors;
	DblVector _fxFwds;
	FwdInterp* _fwdInterp;
};	

class FXBlackPricer: FXPricer {
public:
	FXBlackPricer( const double dtoday, const double dexp, 
			const double fwd, VolNode *vol, const double strike, 
			const double ir, std::string otype, 
			DblVector fxTenors, DblVector fxFwds )
			: FXPricer( dtoday, dexp, fwd, vol, strike, ir, otype, fxTenors, fxFwds) {}
	virtual double price();
};

class FXDigitalPricer: FXPricer {
public:
	FXDigitalPricer( const double dtoday, const double dexp, 
			const double fwd, VolNode *vol, const double strike, 
			const double ir, std::string otype, 
			DblVector fxTenors, DblVector fxFwds )
			: FXPricer( dtoday, dexp, fwd, vol, strike, ir, otype, fxTenors, fxFwds) {}
	virtual double price();
};

class FXStripPricer : public FXPricer {
public:
	FXStripPricer( const double dtoday, 
		const double startDate, const double endDate, 
		const double fwd, VolNode *vol,
		const double strike, const double ir, 
		const std::string otype, const DblVector &hols, 
		DblVector fxTenors, DblVector fxFwds );
	virtual double price();
	virtual void setFwd( const double fwd);
	virtual void setVol( VolNode *vol);
	virtual void setIR( const double ir);
	virtual void setToday( const double dtoday);

private:
	vector<BlackPricer> _pvec;
	DblVector _bdays;
	DblVector _hols;
	double _sDate;
	double _eDate;
};

class DigitalStripPricer : public Pricer {
public:
	DigitalStripPricer( const double dtoday, 
		const double startDate, const double endDate, 
		const double fwd, VolNode *vol,
		const double strike, const double ir, 
		const std::string otype, const DblVector &hols );
	virtual double price();
	virtual void setFwd( const double fwd);
	virtual void setVol( VolNode *vol);
	virtual void setIR( const double ir);
	virtual void setToday( const double dtoday);

private:
	vector<DigitalPricer> _pvec;
	DblVector _bdays;
	DblVector _hols;
	double _sDate;
	double _eDate;
};

#endif
