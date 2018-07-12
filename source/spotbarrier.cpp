#include "spotbarrier.h"

/// down knock-in call
double SpotBarrierSingleFull_dic(double spot,
	double vol,
	double expiry,
	double strike,
	double barrier,
	double r,
	double q)
{
	double price = BSPrice(spot, strike, vol, expiry, r, q, "c") 
					- SpotBarrierSingleFull_doc(spot, vol, expiry, strike, barrier, r, q);
	return price;
}

/// up knock-in call
double SpotBarrierSingleFull_uic(double spot,
	double vol,
	double expiry,
	double strike,
	double barrier,
	double r,
	double q)
{
	double price = BSPrice(spot, strike, vol, expiry, r, q, "c")
		- SpotBarrierSingleFull_uoc(spot, vol, expiry, strike, barrier, r, q);
	return price;
}

/// down knock-in put
double SpotBarrierSingleFull_dip(double spot,
	double vol,
	double expiry,
	double strike,
	double barrier,
	double r,
	double q)
{
	double price = BSPrice(spot, strike, vol, expiry, r, q, "p")
		- SpotBarrierSingleFull_dop(spot, vol, expiry, strike, barrier, r, q);
	return price;
}

/// up knock-in put
double SpotBarrierSingleFull_uip(double spot,
	double vol,
	double expiry,
	double strike,
	double barrier,
	double r,
	double q)
{
	double price = BSPrice(spot, strike, vol, expiry, r, q, "p")
		- SpotBarrierSingleFull_uop(spot, vol, expiry, strike, barrier, r, q);
	return price;
}

/// down knock-out call
double SpotBarrierSingleFull_doc(double spot,
	double vol,
	double expiry,
	double strike,
	double barrier,
	double r,
	double q)
{
	double price = 0.0;
	if (spot <= barrier)   // knock-out region
	{
		price = 0.0;
	}
	else if (vol * vol * expiry <= 0)
	{
		price = BSPrice(spot, strike, vol, expiry, r, q, "c");
	}
	else
	{		
		const double H2_F = barrier * barrier / spot;
		const double mu = -0.5 + (r - q) / (vol * vol);
		const double H_F2mu = std::pow(barrier / spot, 2.0 * mu);		

		if (strike > barrier)
		{			
			price = BSPrice(spot, strike, vol, expiry, r, q, "c") 
				- H_F2mu * BSPrice(H2_F, strike, vol, expiry, r, q, "c");
		}
		else
		{
			price = BSPrice(spot, barrier, vol, expiry, r, q, "c")
				+ (barrier - strike) * BSDigital(spot, barrier, vol, expiry, r, q, "c")
				- H_F2mu * (BSPrice(H2_F, barrier, vol, expiry, r, q, "c")
				+ (barrier - strike) * BSDigital(H2_F, barrier, vol, expiry, r, q, "c"));
		}
	}
	return price;
}

/// up knock-out call
double SpotBarrierSingleFull_uoc(double spot,
	double vol,
	double expiry,
	double strike,
	double barrier,
	double r,
	double q)
{
	double price = 0.0;
	if (spot >= barrier)  // knock-out region
	{
		price = 0.0;
	}
	else if (vol * vol * expiry <= 0)
	{
		price = BSPrice(spot, strike, vol, expiry, r, q, "c");
	}
	else if (strike > barrier)
	{
		price = 0.0;
	}
	else
	{
		const double H2_F = barrier * barrier / spot;
		const double mu = -0.5 + (r - q) / (vol * vol);
		const double H_F2mu = std::pow(barrier / spot, 2.0 * mu);

		price = BSPrice(spot, strike, vol, expiry, r, q, "c")
			- BSPrice(spot, barrier, vol, expiry, r, q, "c")
			- (barrier - strike) * BSDigital(spot, barrier, vol, expiry, r, q, "c")
			- H_F2mu * ( BSPrice(H2_F, strike, vol, expiry, r, q, "c")
			- BSPrice(H2_F, barrier, vol, expiry, r, q, "c")
			- (barrier - strike) * BSDigital(H2_F, barrier, vol, expiry, r, q, "c") );
	}
	return price;
}

/// down knock-out put
double SpotBarrierSingleFull_dop(double spot,
	double vol,
	double expiry,
	double strike,
	double barrier,
	double r,
	double q)
{
	double price = 0.0;	
	if (spot <= barrier)       // knock-out region
	{
		price = 0.0;
	}
	else if (vol * vol * expiry <= 0)
	{
		price = BSPrice(spot, strike, vol, expiry, r, q, "p");
	}
	else if (strike > barrier) // positive time to maturity
	{
		const double H2_F = barrier * barrier / spot;
		const double mu = -0.5 + (r - q) / (vol * vol);
		const double H_F2mu = std::pow(barrier / spot, 2.0 * mu);
		price = BSPrice(spot, strike, vol, expiry, r, q, "p")
			- BSPrice(spot, barrier, vol, expiry, r, q, "p")
			- (strike - barrier) * BSDigital(spot, barrier, vol, expiry, r, q, "p")
			- H_F2mu*(BSPrice(H2_F, strike, vol, expiry, r, q, "p")
			- BSPrice(H2_F, barrier, vol, expiry, r, q, "p")
			- (strike - barrier) * BSDigital(H2_F, barrier, vol, expiry, r, q, "p"));
	}
	else
	{
		price = 0.0;
	}

	return price;
}

/// up knock-out put
double SpotBarrierSingleFull_uop(double spot,
	double vol,
	double expiry,
	double strike,
	double barrier,
	double r,
	double q)
{
	double price = 0.0;
	if (spot >= barrier)  // knock-out region
	{
		price = 0.0;
	}
	else if (vol * vol * expiry <= 0)
	{
		price = BSPrice(spot, strike, vol, expiry, r, q, "p");
	}
	else
	{		
		const double H2_F = barrier * barrier / spot;
		const double mu = -0.5 + (r - q) / (vol * vol);
		const double H_F2mu = std::pow(barrier / spot, 2.0 * mu);		

		if (strike > barrier)
		{
			price = BSPrice(spot, barrier, vol, expiry, r, q, "p")
				+ (strike - barrier)* BSDigital(spot, barrier, vol, expiry, r, q, "p")
				- H_F2mu * (BSPrice(H2_F, barrier, vol, expiry, r, q, "p")
				+ (strike - barrier) * BSDigital(H2_F, barrier, vol, expiry, r, q, "p"));
		}
		else
		{
			price = BSPrice(spot, strike, vol, expiry, r, q, "p")
				- H_F2mu * BSPrice(H2_F, strike, vol, expiry, r, q, "p");
		}
	}

	return price;
}