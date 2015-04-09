#include "interpolator.h"

double Interp::InterpByExpiry(const double dexp) {
	double x = (dexp - this->dtoday_());
	double y = 0.0;
	DblVector* tenors = this->tenors_();
	DblVector* values = this->values_();
	if (tenors->size() == 0)
		y = 0.0;
	else if ((tenors->size() == 1) || (tenors->at(0) >= x))
		y = values->at(0);
	else {
		for (size_t i = 1; i < tenors->size(); ++i) {
			double x1 = tenors->at(i-1);
			double x2 = tenors->at(i);
			double y1 = values->at(i-1);
			double y2 = values->at(i);
			if (x2 >= x) {
				y = (y2 - y1)/(x2 - x1) * (x - x1) + y1;
				break;
			}
			y = y2;
		}
	}
	return y;
}

double VolInterp::InterpByExpiry(const double dexp) {
	double x = (dexp - this->dtoday_());
	double y = 0.0;
	DblVector* tenors = this->tenors_();
	DblVector* values = this->values_();
	if (tenors->size() == 0)
		y = 0.0;
	else if ((tenors->size() == 1) || (tenors->at(0) >= x))
		y = values->at(0);
	else {
		for (size_t i = 1; i < tenors->size(); ++i) {
			double x1 = tenors->at(i-1);
			double x2 = tenors->at(i);
			double y1 = values->at(i-1);
			double y2 = values->at(i);
			y = std::sqrt((y2 * y2 * x2 * (x - x1) + y1 * y1 * x1 * (x2 - x))/(x2 - x1)/x);
			if (x2 >= x) break;
		}
	}
	return y;
}

double FwdInterp::InterpByExpiry(const double dexp) {
	double x = (dexp - this->dtoday_());
	double y = 0.0;
	DblVector* tenors = this->tenors_();
	DblVector* values = this->values_();
	if (tenors->size() == 0)
		y = 0.0;
	else if ((tenors->size() == 1) || (tenors->at(0) >= x))
		y = values->at(0);
	else {
		double spot = values->at(0);
		for (size_t i = 1; i < tenors->size(); ++i) {
			double x1 = tenors->at(i-1);
			double x2 = tenors->at(i);
			double y1 = values->at(i-1);
			double y2 = values->at(i);
			y = (std::log(y2/spot) * (x - x1) + std::log(y1/spot) * (x2 - x))/(x2 - x1);
			y = std::exp(y) * spot;
			if (x2 >= x) break;
		}
	}
	return y;
}
