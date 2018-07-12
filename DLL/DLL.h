#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#include <xlw/CellMatrix.h>
#include <xlw/MyContainers.h>
#include <xlw/DoubleOrNothing.h>
#include <xlw/ArgList.h>
#include "volmodel.h"
#include "pricer.h"
#include "math_utils.h"
#include "timeseries.h"
#include "sqlserveraccess.h"
#include "barriersmile.h"

using namespace xlw;

//<xlw:libraryname=MyTestLibrary

DLL_API MyArray CLFitDelta5VolParams( const double dtoday,
					const double dexp,
					const double fwd,
					const MyArray &strikeList,
					const MyArray &volList);

DLL_API double CLSmileVolbyStrike(const double dtoday,
					const double dexp,
					const double fwd,
					const double atm, 
					const double v90, 
					const double v75, 
					const double v25, 
					const double v10,
					const double K,
					const std::string accrual = "act365");

DLL_API double CLSmileVolbyDelta(const double dtoday,
					const double dexp,
					const double fwd,
					const double atm, 
					const double v90, 
					const double v75, 
					const double v25, 
					const double v10,
					const double delta,
					const std::string accrual = "act365");

DLL_API double CLSmileEuroOptPricer(const double dtoday,
					const double dexp,
					const double fwd,
					const double strike,
					const double atm, 
					const double v90, 
					const double v75, 
					const double v25, 
					const double v10,
					const double doptexp,
					const double ir,
					const std::string otype,
					const std::string outflag,
					const double alpha,
					const double beta,
					const std::string accrual = "act365");

DLL_API double CLEuroOptStripPricer(const double dtoday,
					const double dstart,
					const double dend,
					const double fwd,
					const double strike,
					const double atm, 
					const double v90, 
					const double v75, 
					const double v25, 
					const double v10,
					const double doptexp,
					const double ir,
					const std::string otype,
					const std::string outflag,
					const MyArray &hols,
					const double alpha,
					const double beta,
					const std::string accrual = "act365");

DLL_API double CLSmileBinOptPricer(const double dtoday,
					const double dexp,
					const double fwd,
					const double strike,
					const double atm, 
					const double v90, 
					const double v75, 
					const double v25, 
					const double v10,
					const double doptexp,
					const double ir,
					const std::string otype,
					const std::string outflag,
					const double alpha,
					const double beta,
					const std::string accrual = "act365");

DLL_API double CLDigitalStripPricer(const double dtoday,
					const double dstart,
					const double dend,
					const double fwd,
					const double strike,
					const double atm, 
					const double v90, 
					const double v75, 
					const double v25, 
					const double v10,
					const double doptexp,
					const double ir,
					const std::string otype,
					const std::string outflag,
					const MyArray &hols,
					const double alpha,
					const double beta,
					const std::string accrual = "act365");

DLL_API double CLSprdOptPricer(const double dtoday,
					const double dexp,
					const double fwd,
					const double strike,
					const double atm, 
					const double ir,
					const std::string otype,
					const std::string outflag,
					const std::string accrual = "act365");

DLL_API double CLBlackImpliedVol(const double dtoday, 
					const double dexp, 
					const double fwd, 
					const double strike, 
					const double ir,
					const std::string otype, 
					const double optPrice);

DLL_API double CLBachelierImpliedVol(const double dtoday, 
					const double dexp, 
					const double fwd, 
					const double strike, 
					const double ir,
					const std::string otype, 
					const double prem);

DLL_API double CLBlackDelta(const double dtoday, 
					const double dexp, 
					const double fwd,
					const double strike,
					const double vol,
					const double ir, 
					const std::string otype);

DLL_API double CLBarrierOptFlatVolPricer(const double dtoday,
					const double dexp,
					const double fwd,
					const double strike,
					const double barrier,
					const std::string btype,
					const double atm, 
					const double doptexp,
					const double ir,			
					const std::string otype,
					const std::string mtype,
					const std::string outflag,
					const double alpha,
					const double beta,
					const std::string accrual = "act365");

DLL_API double CLBarrierOptSmilePricer(const double dtoday,
					const double dexp,
					const double fwd,
					const double strike,
					const double barrier,
					const std::string btype,
					const double atm, 
					const double v90, 
					const double v75, 
					const double v25, 
					const double v10,
					const double doptexp,
					const double ir,			
					const std::string otype,
					const std::string mtype,
					const std::string outflag,
					const double alpha,
					const double beta,
					const std::string accrual = "act365");

DLL_API double CLBarrierStripPricer(const double dtoday,
					const double dstart,
					const double dend,
					const double fwd,
					const double strike,
					const double barrier,
					const std::string btype,
					const double atm, 
					const double doptexp,
					const double ir,			
					const std::string otype,
					const std::string mtype,
					const std::string outflag,
					const MyArray &hols,
					const double alpha,
					const double beta,
					const std::string accrual = "act365");

DLL_API DblVectorOfVectorsOfVectors CLCalibHistBreakevenVol( vector<TimeSeries> &ts_vec, 
					int sprdnum, 
					int expbackdays, 
					int freq);