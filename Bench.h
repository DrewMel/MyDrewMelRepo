//#include "IJusWTBServicebinding.nsmap"
#include "stdsoap2.h"
#include "soapIJusWTBServicebindingProxy.h"
#include "Util.h"
#include <fstream>
#include "strs.h"

#pragma once

namespace Pmtrtbl{
	enum {
		TNCOLUMN,
		MinFCOLUMN,
		MaxFCOLUMN,
		MinTCOLUMN,
		MaxTCOLUMN,
		MinVCOLUMN,
		ProofMCOLUMN,
		COLUMN_NUM
	};
}

namespace Rslttbl {

	enum { TNCOLUMN, FLOWRATE, WEIGHT, INLETTEMP, OUTLETTEMP, INLETPRESSURE, OUTLETPRESSURE, LAMBDA, MIDTEMP, MIDPULSECONST, MIDVOLUME, MIDERROR, DURATION, ADDITIVELAMDA, COLUMN_NUM};
}


#define LOG(FOO, STR)					std::cout << TimeStamp() << "    [" << #FOO << ']' <<"[BENCH]"; \
										iBench? std::cout << "[LINE A]" : std::cout << "[LINE B]"; \
										std::cout << STR << std::endl\


#define LOG_ERR(FOO, STR)				std::cout << TimeStamp() << "    [" << #FOO << ']' <<"[BENCH]"; \
										iBench? std::cout << "[LINE A]" : std::cout << "[LINE B]"; \
										std::cout << "[ERROR]: " << STR << std::endl\

#define LOG_TNM_ERR(FOO, TESTNM, STR)	std::cout << TimeStamp() << "    [" << #FOO << ']' <<"[BENCH]";\
										iBench? std::cout << "[LINE A]" :  std::cout << "[LINE B]";\
										std::cout << "[ERROR]: " << TESTNM << " " << STR << std::endl\

#define LOG_SOAP_ERR(FOO, STR)			std::cout << TimeStamp() << "    [" << #FOO << ']' <<"[SOAP]: ";\
										std::cout << STR<< std::endl\

class BenchTest
{
public:
	std::string testName;
	ns3__TTestParameters parameters;
	ns3__TTestResult result;
};

class Bench:private IJusWTBServicebindingProxy
{
private:
	int iBench;
	BenchTest* tests;
	ns4__GetMeasurementDataResponse measinfo;
	
public:
#define LINEA	0
#define LINEB	1
#define CFGFLENME "cfg.txt"

	Bench():tests(nullptr), iBench(-1)
	{
		std::ifstream cfgf;
		cfgf.open(CFGFLENME);
		if (cfgf.is_open())
		{
			std::string lnestr;
			cfgf >> lnestr;
			if (!lnestr.compare("LineA"))
			{
				iBench = LINEA;
			}
			else if(!lnestr.compare("LineB"))
			{
				iBench = LINEB;
			}
		}
		else
		{
			LOG_ERR(Init, "Failed to read configs. Set the name of line to use");
		}

	}

	int setLine(const char* BenchLine);
	//err_handle

	int getTestsInfo (const char* tn);
	int setTestParameter(const int sze, const char* tnkeyval[]);
	int executeTest(const char* tn );
	int getTestResultInfo(const char* tn);
	int deleteTest(const char* tn);
	int stopTest(void);
	~Bench();
private:
	int GetStatus( struct ns4__GetStatusResponse& _param_1);
	int getTests(void);

};

const char* errnameret(enum ns3__TResult err);
