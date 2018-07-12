#ifndef SPOTBARRIER_H
#define SPOTBARRIER_H
#include "euopt.h"

double SpotBarrierSingleFull_dic(double spot,
	double vol,
	double expiry,
	double strike,
	double barrier,
	double r,
	double q);

double SpotBarrierSingleFull_uic(double spot,
	double vol,
	double expiry,
	double strike,
	double barrier,
	double r,
	double q);

double SpotBarrierSingleFull_dip(double spot,
	double vol,
	double expiry,
	double strike,
	double barrier,
	double r,
	double q);

double SpotBarrierSingleFull_uip(double spot,
	double vol,
	double expiry,
	double strike,
	double barrier,
	double r,
	double q);

double SpotBarrierSingleFull_doc(double spot,
	double vol,
	double expiry,
	double strike,
	double barrier,
	double r,
	double q);

double SpotBarrierSingleFull_uoc(double spot,
	double vol,
	double expiry,
	double strike,
	double barrier,
	double r,
	double q);

double SpotBarrierSingleFull_dop(double spot,
	double vol,
	double expiry,
	double strike,
	double barrier,
	double r,
	double q);

double SpotBarrierSingleFull_uop(double spot,
	double vol,
	double expiry,
	double strike,
	double barrier,
	double r,
	double q);


#endif