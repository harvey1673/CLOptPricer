#ifndef SQLSERVERACCESS_H
#define SQLSERVERACCESS_H

#include "timeseries.h"

TimeSeries GetDBTimeSeries(std::string hostname, std::string dbname, std::string tablename, int ContDate);

void WriteVolResultsToDB(std::string hostname, std::string dbname, std::string tablename, DblVector res, std::string mkt, std::string voltype, int freq, int cont, int sprdTernor);

#endif
