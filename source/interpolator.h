#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H
#include <cmath>
#include "math_utils.h"

class Interp {
public:
	Interp(dtoday, DblVector *tenors, DblVector *values):_tenors(tenors), _values(values), _dtoday(dtoday) {}
	virtual double InterpByExpiry(const double dexp);
	void setToday( double dtoday ) { _dtoday = dtoday; }
	double dtoday_() { return _dtoday; }
private:
	double _dtoday; 
	DblVector *_tenors;
	DblVector *_values;
};


double Interp::InterpByExpiry(const double dexp) {
	double x = (dexp - _dtoday);
	double y = 0.0;
	if (_tenors->size() == 0)
		y = 0.0;
	else if (_tenors->size() == 1) || (_tenors->at(0) >= x)
		y = _values->at(0);
	else {
		for (size_t i = 1; i < _tenors->size(); ++i) {
			double x1 = _tenors->at(i-1);
			double x2 = _tenors->at(i);
			double y1 = _values->at(i-1);
			double y2 = _values->at(i);
			if (x2 >= x) {
				y = (y2 - y1)/(x2 - x1) * (x - x1) + y1;
				break;
			}
			y = y2;
		}
	}
	return y;
}

class VolInterp: public Interp {
public:
	virtual double InterpByExpiry(const double dexp);
};

double VolInterp::InterpByExpiry(const double dexp) {
	double x = (dexp - _dtoday);
	double y = 0.0;
	if (_tenors.size() == 0)
		y = 0.0;
	else if (_tenors->size() == 1) || (_tenors->at(0) >= x)
		y = _values->at(0);
	else {
		for (size_t i = 1; i < _tenors->size(); ++i) {
			double x1 = _tenors->at(i-1);
			double x2 = _tenors->at(i);
			double y1 = _values->at(i-1);
			double y2 = _values->at(i);
			y = std::sqrt((y2 * y2 * x2 * (x - x1) + y1 * y1 * x1 * (x2 - x))/(x2 - x1)/x);
			if (x2 >= x) break;
		}
	}
	return y;
}

class FwdInterp: public Interp {
public:
	virtual double InterpByExpiry(const double dexp);
};

double FwdInterp::InterpByExpiry(const double dexp) {
	double x = (dexp - _dtoday);
	double y = 0.0;
	if (_tenors.size() == 0)
		y = 0.0;
	else if (_tenors.size() == 1) || (_tenors[0] >= x)
		y = _values[0];
	else {
		double spot = _values[0];
		for (size_t i = 1; i < _tenors.size(); ++i) {
			double x1 = _tenors->at(i-1);
			double x2 = _tenors->at(i);
			double y1 = _values->at(i-1);
			double y2 = _values->at(i);
			y = (std::log(y2/spot) * (x - x1) + std::log(y1/spot) * (x2 - x))/(x2 - x1);
			y = std::exp(y) * spot;
			if (x2 >= x) break;
		}
	}
	return y;
}
#endif
