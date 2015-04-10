#ifndef FXPRICER_H
#define FXPRICER_H
#include "pricer.h"

class FXPricer : public Pricer {
public:
	FXPricer(const double dtoday, const double dexp, 
			const double fwd, VolNode *vol, const double strike, const double ir, 
			std::string otype, DblVector fxTenors, DblVector fxFwds, VolNode *fxvol);
	double GetFXFwdByDate(const double dexp) { return _fwdInterp->InterpByExpiry(dexp); }
	virtual double fxdelta();
	virtual double fxdeltas();
	virtual double fxvega();
	virtual double fxvegas();
	
private:
	DblVector _fxTenors;
	DblVector _fxFwds;
	FwdInterp* _fwdInterp;
};	

#endif
