#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H
#include <cmath>
#include <cstddef>
#include "math_utils.h"

class Interp {
public:
	Interp(double dtoday, DblVector* tenors, DblVector* values):_tenors(tenors), _values(values), _dtoday(dtoday) {}
	Interp():_tenors(NULL), _values(NULL), _dtoday(0.0) {}
	virtual double InterpByExpiry(const double dexp);
	void setToday( double dtoday ) { _dtoday = dtoday; }
	double dtoday_() { return _dtoday; }
	DblVector* tenors_() { return _tenors; }
	DblVector* values_() { return _values; }
private:
	double _dtoday; 
	DblVector* _tenors;
	DblVector* _values;
};

class VolInterp: public Interp {
public:
	VolInterp(double dtoday, DblVector* tenors, DblVector* values): Interp(dtoday, tenors, values) {}
	double InterpByExpiry(const double dexp);
};

class FwdInterp: public Interp {
public:
	FwdInterp(double dtoday, DblVector* tenors, DblVector* values): Interp(dtoday, tenors, values) {}
	double InterpByExpiry(const double dexp);
};

#endif
