#include "fxpricer.h"

FXPricer::FXPricer(const double dtoday, const double dexp, 
			const double fwd, VolNode *vol, const double strike, const double ir, 
			std::string otype, DblVector fxTenors, DblVector fxFwds, VolNode *fxvol) :
			Pricer(dtoday, dexp, fwd, vol, strike, ir, otype), _fxTenors(fxTenors), _fxFwds(fxFwds), _fxVol(fxvol)
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
	//VolNode* vol = _fxVol->;
	//double atmvol = vol->atmVol_();
	//double eps = this->volTweak();

	//double uvol = (1 + eps) * atmvol;
	//vol->setAtm(uvol);
	//double uprice = this->price();

	//double dvol = (1 - eps) * atmvol;
	//vol->setAtm(dvol);
	//double dprice = this->price();

	//vol->setAtm(atmvol);

	//return (uprice - dprice)/(uvol - dvol);
	return 0.0;
}

DblVector FXPricer::fxvegas()
{
	DblVector vegas;
	//VolNode* vol = _fxVol->;
	//double atmvol = vol->atmVol_();
	//double eps = this->volTweak();

	//double uvol = (1 + eps) * atmvol;
	//vol->setAtm(uvol);
	//double uprice = this->price();

	//double dvol = (1 - eps) * atmvol;
	//vol->setAtm(dvol);
	//double dprice = this->price();

	//vol->setAtm(atmvol);

	//return (uprice - dprice)/(uvol - dvol);
	return vegas;
}
