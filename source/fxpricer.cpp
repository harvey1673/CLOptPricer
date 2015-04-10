#include "fxpricer.h"

FXPricer::FXPricer(const double dtoday, const double dexp, 
			const double fwd, VolNode *vol, const double strike, const double ir, 
			std::string otype, DblVector fxTenors, DblVector fxFwds) :
			Pricer( dtoday, dexp, fwd, vol, strike, ir, otype), _fwdTenors(fxTenors), _fxFwds(fxFwds)
{
	_fwdInterp = new FwdInterp(dtoday, &_fwdTenors, &_fxFwds);
}

double FXPricer::fxdelta()
{
	DblVector fxfwd = this->_fxFwds;
	double eps = this->priceTweak();
	double spot = fxFwd[0];
	for (size_t i = 0; i < _fxFwds.size(); ++i)
		_fwFwds[i] = fxfwd[i] * ( 1 + eps );
	double uprice = this->price();
	for (size_t i = 0; i < _fxFwds.size(); ++i)
		_fwFwds[i] = fxfwd[i] * ( 1 - eps );
	double dprice = this->price();
	for (size_t i = 0; i < _fxFwds.size(); ++i)
		_fwFwds[i] = fxfwd[i];
	return (uprice - dprice)/(2*eps*spot);
}

double FXPricer::fxdeltas()
{
	DblVector deltas;
	DblVector fxfwds = this->_fxFwds;
	double eps = this->priceTweak();
	for (size_t i = 0; i < _fxFwds.size(); ++i)
	{
		_fwFwds[i] = fxfwds[i] * ( 1 + eps );
		double uprice = this->price();
		_fwFwds[i] = fxfwds[i] * ( 1 - eps );
		double dprice = this->price();
		_fwFwds[i] = fxfwds[i];
		deltas.push_back((uprice - dprice)/(2*eps*fwfwds[i]));
	}
	return deltas;
}

double FXPricer::fxvega()
{
	VolNode* vol = _fxvol->vol_();
	double atmvol = vol->atmVol_();
	double eps = this->volTweak();

	double uvol = (1 + eps) * atmvol;
	vol->setAtm(uvol);
	double uprice = this->price();

	double dvol = (1 - eps) * atmvol;
	vol->setAtm(dvol);
	double dprice = this->price();

	vol->setAtm(atmvol);

	return (uprice - dprice)/(uvol - dvol);
}
