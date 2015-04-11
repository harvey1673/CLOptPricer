#include "sqlserveraccess.h"

#include <stdio.h> 
#include <tchar.h> 

#include <comdef.h> 
#include <gcroot.h> 
#include <iostream> 
#include <string.h>
#include <stdlib.h>
#include <msclr\marshal_cppstd.h>

#using <System.dll> 
#using <System.Data.dll> 
using namespace System; 
using namespace System::Data; 
using namespace System::Data::SqlClient; 
using namespace System::Runtime::InteropServices; 
using namespace msclr::interop;

TimeSeries GetDBTimeSeries(std::string hostname, std::string dbname, std::string tablename, int ContDate)
{
	int nDay, nMonth, nYear;
	ExcelSerialDateToDMY(ContDate, nDay, nMonth, nYear);
	DateTime^ clrDate = gcnew DateTime(nYear, nMonth, nDay);
	String^ hostString = gcnew String(hostname.c_str());
	String^ dbString = gcnew String(dbname.c_str());
	String^ tableString = gcnew String(tablename.c_str());
	String^ qString = "SELECT Date, PX_Settle FROM dbo." + tableString + " WHERE ProductID=1 and Prompt='" + clrDate->ToString("yyyyMMdd") + "' ORDER by date";
	SqlConnection^ myConnection = gcnew SqlConnection("Data Source=" + hostString + ";Initial Catalog=" + dbString + ";Integrated Security=SSPI;");
	myConnection->Open();

	DblVector tsData, tsDate;
	SqlCommand^ scmd = gcnew SqlCommand(qString, myConnection);
	SqlDataReader^ r = scmd->ExecuteReader();
	while (r->Read())
	{
		DateTime date = r->GetDateTime(0);
		double tmpdate = DMYToExcelSerialDate(date.Day, date.Month, date.Year);		
		tsDate.push_back(tmpdate);
		double tmpdata = r->GetDouble(1);
		tsData.push_back(tmpdata);
	}
	
	r->Close();
	return TimeSeries(tsDate, tsData);
}

void WriteVolResultsToDB(std::string hostname, std::string dbname, std::string tablename, DblVector res, std::string mkt, std::string voltype, int freq, int cont, int sprdTenor)
{
	int nDay, nMonth, nYear;
	String^ hostString = gcnew String(hostname.c_str());
	String^ dbString = gcnew String(dbname.c_str());
	String^ tableString = gcnew String(tablename.c_str());

	ExcelSerialDateToDMY(cont, nDay, nMonth, nYear);
	DateTime^ contDate = gcnew DateTime(nYear, nMonth, nDay);
	String^ qString = "INSERT INTO dbo."+ tableString + " (Market, VolType, Freq, Contract, SpreadTenor, VolTenor, VolMark) VALUES (@Market, @VolType, @Freq, @Contract, @SpreadTenor, @VolTenor, @VolMark)";
	SqlConnection^ myConnection = gcnew SqlConnection("Data Source=" + hostString + ";Initial Catalog=" + dbString + ";Integrated Security=SSPI;");
	myConnection->Open();

	String^ mktString = gcnew String(mkt.c_str());
	String^ voltypeString = gcnew String(voltype.c_str());

	for (unsigned int i = 0; i < res.size(); ++i)
	{
		SqlCommand^ scmd = gcnew SqlCommand(qString, myConnection);
		scmd->Parameters->AddWithValue("@Market", mktString);
		scmd->Parameters->AddWithValue("@VolType", voltypeString);
		scmd->Parameters->AddWithValue("@Freq", freq);
		scmd->Parameters->AddWithValue("@Contract", contDate->ToString("yyyyMMdd"));
		scmd->Parameters->AddWithValue("@SpreadTenor", sprdTenor);
		scmd->Parameters->AddWithValue("@VolTenor", i+1);
		scmd->Parameters->AddWithValue("@VolMark", res[i]);
		scmd->ExecuteNonQuery();
	}
	myConnection->Close();
}
