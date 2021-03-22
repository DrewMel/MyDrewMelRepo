#include <iostream>
#include "Bench.h"

#include  <string>
#include  <iomanip>
#include <fstream>

#define TAB					"    "

const char* errnameret(enum ns3__TResult err);
//const char* stateret(enum ns3__TStatusCode stt);

Bench::~Bench()
{
	delete[]tests;
}

const char* bnchsts[] = {
							"Program is ready to start testing",
							"[ERROR]: Program or device in an error state ( central stop, etc. )",
							"Program is performing testing, but actually not measuring",
							"Program is actually measuring the meters",
							"Waiting for the Continue function",
							"Waiting for the Continue function",
							"Program is performing some process, other than a test",
							"[ERROR]: Unknown error!"
};

int Bench::GetStatus(struct ns4__GetStatusResponse& stsrsp)
{
	int err = IJusWTBServicebindingProxy::GetStatus(iBench, stsrsp);
	if (SOAP_OK != err)
	{
		LOG_SOAP_ERR(GetStatus, err);
		return -1;
	}
	else if (stsrsp.return_)
	{
		if (ns3__TStatusCode__stBusy < stsrsp.return_->StatusCode)
		{
			LOG(GetStatus, stsrsp.return_->TestName << ": " << bnchsts[ns3__TStatusCode__stBusy + 1]);
		}
		else
		{
			LOG(GetStatus, stsrsp.return_->TestName << ": " << bnchsts[stsrsp.return_->StatusCode]);
		}
		LOG(GetStatus, "MESSAGE: " << stsrsp.return_->StatusMessage);
		return 0;
	}
	return -1;
}

int Bench::getTests(void)
{
	int err;
	for (int i = 0; i < 4; i++)
	{
		err = GetMeasurementData(iBench, measinfo);
		Sleep(1000);

		if (SOAP_OK != err)
		{
			LOG_SOAP_ERR(GetMeasurementData, err);
			return -1;
		}

		if (nullptr == measinfo.return_)
		{
			LOG_ERR(GetMeasurementData, "Bench parameter is not valid!");
			return -1;
		}

		if (measinfo.return_->TypeName.empty())
		{
			LOG_ERR(GetMeasurementData, "Сounter type is not specified! Searching types...");
			ns4__GetTypeListResponse tlresp;
			err = this->GetTypeList(tlresp);
			Sleep(1000);

			if (!tlresp.return_->__size)
			{
				LOG_ERR(GetTypeList, "List of types is empty!");
				return -1;
			}
			ns4__NewMeasurementResponse nmresp;
			err = NewMeasurement(iBench, tlresp.return_->__ptr[0], false, nmresp);
			Sleep(1000);
			return -1;
		}
		else
		{
			break;
		}

	}

	const int& test_num = this->measinfo.return_->TestNames->__size;
	this->tests = new BenchTest[test_num];
	ns4__GetTestParametersResponse testParameters;
	for (int i = 0; i < test_num; i++)
	{
		tests[i].testName = measinfo.return_->TestNames->__ptr[i];
		Sleep(1000);
		err = GetTestParameters(iBench, tests[i].testName, testParameters);
		if (SOAP_OK != err)
		{
			LOG_SOAP_ERR(GetTestParameters, err);
			return -1;
		}

		if (nullptr == testParameters.return_)
		{
			LOG_TNM_ERR(GetTestParameters, tests[i].testName, " is not valid name!");
			return -1;
		}

		tests[i].parameters = *testParameters.return_;
	}
}


const std::string subDictionary[] =
{
	KEYMinFCOLUMNSTR,
	KEYMaxFCOLUMNSTR,
	KEYMinTCOLUMNSTR,
	KEYMaxTCOLUMNSTR,
	KEYMinVCOLUMNSTR,
	KEYProofMCOLUMNSTR
};

#define DICTSIZE sizeof(subDictionary)/sizeof(subDictionary[0])

const char* proofmethods[PMETHODSNUM] = {
										PMETHODSTR1,
										PMETHODSTR2,
										PMETHODSTR3,
										PMETHODSTR4
};

int Bench::getTestsInfo(const char* tn) 
{
	if (nullptr != tn)
	{
		int err = getTests();
		if (SOAP_OK != err)
		{
			LOG_SOAP_ERR(getTests, err);
			return -1;
		}
	}

	size_t maxcolumnsize[Pmtrtbl::COLUMN_NUM];
	maxcolumnsize[Pmtrtbl::TNCOLUMN] = sizeof(KPNAME);
	maxcolumnsize[Pmtrtbl::MinFCOLUMN] = sizeof(MinFCOLUMNSTR);
	maxcolumnsize[Pmtrtbl::MaxFCOLUMN] = sizeof(MaxFCOLUMNSTR);
	maxcolumnsize[Pmtrtbl::MinTCOLUMN] = sizeof(MinTCOLUMNSTR);
	maxcolumnsize[Pmtrtbl::MaxTCOLUMN] = sizeof(MaxTCOLUMNSTR);
	maxcolumnsize[Pmtrtbl::MinVCOLUMN] = sizeof(MinVCOLUMNSTR);
	maxcolumnsize[Pmtrtbl::ProofMCOLUMN] = sizeof(ProofMCOLUMNSTR) > sizeof(PMETHODSTR1) ? sizeof(ProofMCOLUMNSTR) : sizeof(PMETHODSTR1);

	const int& test_num = this->measinfo.return_->TestNames->__size;
	std::string** table = new std::string* [test_num+1];

	for (int i = 0; i < test_num; i++)
	{
		table[i] = new std::string[Pmtrtbl::COLUMN_NUM];
		maxcolumnsize[Pmtrtbl::TNCOLUMN] = maxintval((UINT)tests[i].testName.size(), maxcolumnsize[Pmtrtbl::TNCOLUMN]);

		table[i][Pmtrtbl::TNCOLUMN] = tests[i].testName;

		table[i][Pmtrtbl::MinFCOLUMN] = std::to_string(tests[i].parameters.MinFlowrate);
		table[i][Pmtrtbl::MaxFCOLUMN] = std::to_string(tests[i].parameters.MaxFlowrate);
		table[i][Pmtrtbl::MinTCOLUMN] = std::to_string(tests[i].parameters.MinTemperature);
		table[i][Pmtrtbl::MaxTCOLUMN] = std::to_string(tests[i].parameters.MaxTemperature);
		table[i][Pmtrtbl::MinVCOLUMN] = std::to_string(tests[i].parameters.MinVolume);
		table[i][Pmtrtbl::ProofMCOLUMN] = proofmethods[tests[i].parameters.ProofMethod];
	}

	char Letter[] = "A";
	if (iBench) Letter[0] = 'B';
	std::cout << measinfo.return_->ProtocolNumber << std::endl;
	std::cout << "LINE " << Letter << std::endl;

	spc space;
	std::cout << space.getnspcs(maxcolumnsize[Pmtrtbl::TNCOLUMN]) << TAB <<FirstRow << std::endl;
	std::cout << std::setw(maxcolumnsize[Pmtrtbl::TNCOLUMN]) << std::left <<  KPNAME TAB;

	std::cout << TAB << space.getnspcs((sizeof(MinFCOLUMNSTR) - sizeof(KEYMinFCOLUMNSTR))/2) << KEYMinFCOLUMNSTR;
	std::cout << space.getnspcs(sizeof(MinFCOLUMNSTR) - (sizeof(MinFCOLUMNSTR) - sizeof(KEYMinFCOLUMNSTR)) / 2 - sizeof(KEYMinFCOLUMNSTR));

	std::cout << TAB << space.getnspcs((sizeof(MaxFCOLUMNSTR) - sizeof(KEYMaxFCOLUMNSTR))/2) << KEYMaxFCOLUMNSTR;
	std::cout << space.getnspcs(sizeof(MaxFCOLUMNSTR) - (sizeof(MaxFCOLUMNSTR) - sizeof(KEYMaxFCOLUMNSTR)) / 2 - sizeof(KEYMaxFCOLUMNSTR));

	std::cout << TAB << space.getnspcs((sizeof(MinTCOLUMNSTR) - sizeof(KEYMinTCOLUMNSTR)) / 2) << KEYMinTCOLUMNSTR;
	std::cout << space.getnspcs(sizeof(MinTCOLUMNSTR) - (sizeof(MinTCOLUMNSTR) - sizeof(KEYMinTCOLUMNSTR)) / 2 - sizeof(KEYMinTCOLUMNSTR));

	std::cout << TAB << space.getnspcs((sizeof(MaxTCOLUMNSTR) - sizeof(KEYMaxTCOLUMNSTR)) / 2) << KEYMaxTCOLUMNSTR;
	std::cout << space.getnspcs(sizeof(MaxTCOLUMNSTR) - (sizeof(MaxTCOLUMNSTR) - sizeof(KEYMaxTCOLUMNSTR)) / 2 - sizeof(KEYMaxTCOLUMNSTR));

	std::cout << TAB << space.getnspcs((sizeof(MinVCOLUMNSTR) - sizeof(KEYMinVCOLUMNSTR)) / 2) << KEYMinVCOLUMNSTR;
	std::cout << space.getnspcs(sizeof(MinVCOLUMNSTR) - (sizeof(MinVCOLUMNSTR) - sizeof(KEYMinVCOLUMNSTR)) / 2 - sizeof(KEYMinVCOLUMNSTR));

	std::cout << std::endl;

	for (int i = 0; i < test_num; i++)
	{
		if (tn)
		{
			if (this->tests[i].testName.compare(tn))
			{
				continue;
			}
		}
		std::cout << std::setw(maxcolumnsize[Pmtrtbl::TNCOLUMN]) << std::left << table[i][Pmtrtbl::TNCOLUMN];
		for (int j = Pmtrtbl::MinFCOLUMN; j < Pmtrtbl::COLUMN_NUM; j++)
		{
			std::cout << TAB << std::setw(maxcolumnsize[j] - table[i][j].size()) << std::right << table[i][j] ;
		}
		std::cout << std::endl;
	}

}

#define TESTS_MAX 64
int Bench::setTestParameter(const int sze, const char* tnkeyval[])
{
	bool CreateNewName = false;
	bool CreateNewTEST = true;
	BenchTest* newchgdtest;
	newchgdtest = new BenchTest;

	if (/*tnkeyval[0][0] == '-'*/	strcmp(tnkeyval[0], KEYMinFCOLUMNSTR) && \
									strcmp(tnkeyval[0], KEYMaxFCOLUMNSTR) && \
									strcmp(tnkeyval[0], KEYMinTCOLUMNSTR) && \
									strcmp(tnkeyval[0], KEYMaxTCOLUMNSTR) && \
									strcmp(tnkeyval[0], KEYMinVCOLUMNSTR) && \
									strcmp(tnkeyval[0], KEYProofMCOLUMNSTR))
	{
		//имя теста не может иметь такое же имя, как у ключа 
		
		CreateNewName = true;
	}
	else
	{
		newchgdtest->testName = tnkeyval[0];
	}

	if (!getTests()) return -1;

	int i_tries = 0;
	do
	{
		if (++i_tries > TESTS_MAX)
		{
			LOG_ERR(setTestParameter, "You can create no more then " defnum2text(TESTS_MAX) " tests!");
			break;
		}

		if (CreateNewName)
		{
			char Letter = 'A';
			if (iBench) Letter = 'B';
			newchgdtest->testName = "Test";
			newchgdtest->testName += Letter;
			newchgdtest->testName += std::to_string(i_tries);
		}

		for (int j = 0 + CreateNewName ? 1 : 0; j < sze; j += 2)
		{
			if (nullptr == tests) break;
			if (newchgdtest->testName.compare(tests[j].testName))
			{
				if (CreateNewName)
				{	
					newchgdtest->testName.clear();
				}
				else
				{
					CreateNewTEST = false;
					delete newchgdtest;
					newchgdtest = tests+j;
				}
				break;
			}
		}
	} while (!newchgdtest->testName.empty());

	struct {
		bool used;
		std::string keynm;
		void* val;
	}keyvals[Pmtrtbl::COLUMN_NUM] = {
		{false, KEYMinFCOLUMNSTR, &(newchgdtest->parameters.MinFlowrate)},
		{false, KEYMaxFCOLUMNSTR, &(newchgdtest->parameters.MaxFlowrate)},
		{false, KEYMinTCOLUMNSTR, &newchgdtest->parameters.MinTemperature},
		{false, KEYMaxTCOLUMNSTR, &newchgdtest->parameters.MaxTemperature},
		{false, KEYMinVCOLUMNSTR, &newchgdtest->parameters.MinVolume},
		{false, KEYProofMCOLUMNSTR, &newchgdtest->parameters.ProofMethod}
	};

	bool WrongKeyName;
	for (int i = 0 + CreateNewName? 1 : 0; i < sze; i + 2)
	{
		WrongKeyName = true;
		for (int j = 0; j < Pmtrtbl::COLUMN_NUM; j++)
		{
			if (0==keyvals[j].keynm.compare(tnkeyval[i]))
			{
				WrongKeyName = false;
				if (keyvals[j].used)
				{
					LOG_ERR(setTestParameter, "The key " << keyvals[0].keynm << " is repeated twice or more!");
					return -1;
				}
				keyvals[j].used = true;

				if (j == Pmtrtbl::ProofMCOLUMN)
				{
					for (int k = 0; k < PMETHODSNUM; k++)
					{
						if (!strcmp(proofmethods[k], tnkeyval[i + 1]))
						{
							*(ns3__TProofMethod*)keyvals[j].val = (ns3__TProofMethod)k;
							break;
						}

					}
					if (strcmp(proofmethods[0], tnkeyval[i + 1]) && \
						strcmp(proofmethods[1], tnkeyval[i + 1]) && \
						strcmp(proofmethods[2], tnkeyval[i + 1]) && \
						strcmp(proofmethods[3], tnkeyval[i + 1]))
					{
						LOG_ERR(setTestParameter, tnkeyval[i + 1] << " is incorrect name of method!");
						return -1;
					}
				}
				else
				{
					if (!str2double(tnkeyval[i + 1], *((double*)keyvals[j].val)))
					{
						LOG_ERR(setTestParameter, tnkeyval[i + 1] << " is incorrect!");
						return -1;
					}
				}
				break;
			}		
		}

		if (WrongKeyName)
		{
			LOG_ERR(setTestParameter, tnkeyval[i] << " is wrong  key!");
			return -1;
		}
	}

	//в конце
	if (CreateNewTEST)
	{
		if (sze < ((!CreateNewName) ? 1 : 0) + 2 * Pmtrtbl::COLUMN_NUM)
		{
			LOG_TNM_ERR(setTestParameter, newchgdtest->testName, "Too few parameters for new test!");
			return -1;
		}else if(sze > ((!CreateNewName) ? 1 : 0) + 2 * Pmtrtbl::COLUMN_NUM)
		{
			LOG_TNM_ERR(setTestParameter, newchgdtest->testName, "Too many parameters for new test!");
			return -1;
		}
		
		
	}

	ns4__SetTestParametersResponse stpresp;
	int err = SetTestParameters(iBench, newchgdtest->testName, &newchgdtest->parameters, CreateNewTEST, stpresp);
	
	if (SOAP_OK != err)
	{
		LOG_SOAP_ERR(GetMeasurementData, err);
		delete newchgdtest;
		return -1;
	}

	if (stpresp.return_ > ns3__TResult__rsOK)
	{
		LOG_TNM_ERR(setTestParameter, newchgdtest->testName, errnameret(stpresp.return_));
		delete newchgdtest;
		return -1;
	}
	else
	{
		LOG(setTestParameter, errnameret(stpresp.return_));

		getTestsInfo(newchgdtest->testName.c_str()); 
		//TO DO:
		//проверки для getTestsInfo
		delete newchgdtest;
		return 0;
	}
}

int Bench::executeTest(const char* tn /*= nullptr*/)
{
	ns4__GetStatusResponse gsresp;
	std::string testname(tn);
	ns4__StartResponse srresp;
	ns4__ContinueResponse cresponse;
	ns3__TWaitEvents startevent;
	startevent.StartTest = true;
	startevent.StopTest = true;
	int err = 0;
	do
	{
		if (this->GetStatus(gsresp)) return -1;

		if (!gsresp.return_)
		{
			LOG_TNM_ERR(executeTest, tn, "The Bench parameter is not valid!");
			return -1;
		}
		Sleep(1000);

		if ((tn)&&(gsresp.return_->TestName.compare(tn)))
		{
			LOG_TNM_ERR(executeTest, tn, "Testbench is executing other test - " << gsresp.return_->TestName <<  ". To stop executing press Ctrl+C!");
		}
		switch (gsresp.return_->StatusCode)
		{
		case ns3__TStatusCode__stReady:
			if (tn)
			{
				LOG(executeTest, "Tring to start test " << tn << ".");
			}
			else
			{
				LOG(executeTest, "Tring to start tests.");
			}
			err = this->Start(iBench, testname, &startevent, srresp);
			if (SOAP_OK != err)
			{
				LOG_SOAP_ERR(Start, err);
				return -1;
			}
			break;
		case ns3__TStatusCode__stStartTestWait:
		case ns3__TStatusCode__stStopTestWait:
			err = this->Continue(iBench, cresponse);
			if (SOAP_OK != err)
			{
				LOG_SOAP_ERR(Continue, err);
				return -1;
			}
			break;
		case ns3__TStatusCode__stError:
		default:
			return -1;
		}

		if (gsresp.return_->FinishTestEvent)
		{
			struct ns4__GetTestResultResponse gttstresp;
			err = GetTestResult(iBench, testname, gttstresp);
			if (SOAP_OK != err)
			{
				LOG_ERR(GetTestResult, err);
				return -1;
			}
			if (!gttstresp.return_)
			{
				LOG_TNM_ERR(GetTestResult, gsresp.return_->TestName, "Bench Parameter is not valid or the TestName parameter is a nonexistent or undone test");
			}


			err = getTestsInfo(testname.c_str());
		}

		//TODO: this->GetTestConfiguration

	} while (gsresp.return_->FinishEvent);


}

const char* column_names[] = {
								FLOWRATESTR,
								WEIGHTSTR,
								INLETTEMPSTR,
								OUTLETTEMPSTR,
								INLETPRESSURESTR,
								OUTLETPRESSURESTR,
								LAMBDASTR,
								MIDTEMPSTR,
								MIDPULSECONSTSTR,
								MIDVOLUMESTR,
								MIDERRORSTR,
								DURATIONSTR,
								ADDITIVELAMDASTR
};

#define TABLEHEAD 0

int Bench::getTestResultInfo(const char* tn)
{
	if (this->tests == nullptr)
	{
		LOG_ERR(getTestResultInfo, "Programm has not information about tests");
		return -1;
	}
	size_t maxcolumnsize[Rslttbl::COLUMN_NUM];

	const int& test_num = this->measinfo.return_->TestNames->__size;
	std::string** table = new std::string * [test_num + 1];
	table[TABLEHEAD] = new std::string[Rslttbl::COLUMN_NUM];
	table[TABLEHEAD][Rslttbl::TNCOLUMN].clear();
	for (int i = Rslttbl::FLOWRATE; i < Rslttbl::COLUMN_NUM; i++)
	{
		table[TABLEHEAD][i] = column_names[i];
	}

	for (int i = TABLEHEAD+1; i < test_num; i++)
	{
		table[i] = new std::string[Rslttbl::COLUMN_NUM];
		
		table[i][Rslttbl::TNCOLUMN] = tests[i].testName;
		table[i][Rslttbl::FLOWRATE] = std::to_string(tests[i].result.Flowrate);
		table[i][Rslttbl::WEIGHT] = std::to_string(tests[i].result.Weight);
		table[i][Rslttbl::INLETTEMP] = std::to_string(tests[i].result.InletTemp);
		table[i][Rslttbl::OUTLETTEMP] = std::to_string(tests[i].result.OutletTemp);
		table[i][Rslttbl::INLETPRESSURE] = std::to_string(tests[i].result.InletPressure);
		table[i][Rslttbl::OUTLETPRESSURE] = std::to_string(tests[i].result.OutletPressure);
		table[i][Rslttbl::LAMBDA] = std::to_string(tests[i].result.Lambda);
		table[i][Rslttbl::MIDTEMP] = std::to_string(tests[i].result.MIDTemp);
		table[i][Rslttbl::MIDPULSECONST] = std::to_string(tests[i].result.MIDPulseConst);
		table[i][Rslttbl::DURATION] = std::to_string(tests[i].result.Duration);
		table[i][Rslttbl::ADDITIVELAMDA] = std::to_string(tests[i].result.AdditiveLambda);
	}

	for (int j = 0; j < Rslttbl::COLUMN_NUM; j++)
	{
		for (int i = 0; i < test_num+1; i++)
		{
			maxcolumnsize[j] = maxintval(table[i][j].size(), maxcolumnsize[j]);
		}
	}

	spc space;
	for (int i = 0; i < test_num; i++)
	{
		if (tn)
		{
			if (this->tests[i].testName.compare(tn))
			{
				continue;
			}
		}
		std::cout << table[i][Rslttbl::TNCOLUMN] << space.getnspcs(maxcolumnsize[i] - table[i][Rslttbl::TNCOLUMN].size()) << TAB;
		for (int j = Rslttbl::FLOWRATE; j < Rslttbl::COLUMN_NUM; j++)
		{
			std::cout << space.getnspcs((maxcolumnsize[j] - table[i][j].size()) / 2) << table[i][j] << space.getnspcs(maxcolumnsize[j] - (maxcolumnsize[j] - table[i][j].size()) / 2 - table[i][j].size()) << TAB;
		}
		std::cout << std::endl;
	}
	return 0;
}

int Bench::deleteTest(const char* tn)
{
	if (this->getTests())
	{
		ns4__NewMeasurementResponse nmresp;
		int err = NewMeasurement(iBench, this->measinfo.return_->TypeName, false, nmresp);
		Sleep(1000);
		if (SOAP_OK != err)
		{
			LOG_SOAP_ERR(GetStatus, err);
			return -1;
		}

		if (ns3__TStatusCode__stBusy < nmresp.return_)
		{
			LOG(GetStatus, bnchsts[ns3__TStatusCode__stBusy + 1]);
		}
		else
		{
			LOG(GetStatus, bnchsts[nmresp.return_]);
		}

		return 0;

		if (nullptr == tn)
		{
			if (nullptr == this->tests)
			{
#if 0
				LOG(deleteTest, "There is no test for deleting!");
#endif
				return 0;
			}

			const int& test_num = this->measinfo.return_->TestNames->__size;
			BenchTest* temp = new BenchTest[test_num - 1];

			for (int i = 0; i < test_num; i++)
			{
				if (!tests[i].testName.compare(tn))continue;
				temp[i] = tests[i];
				ns4__SetTestParametersResponse stpresp;
				int err = SetTestParameters(iBench, temp[i].testName, &temp[i].parameters, true, stpresp);
			}
			delete[] tests;
			tests = temp;
		}
		else
		{
			delete[] tests;
			tests = nullptr;
		}

	}
}

int Bench::stopTest(void)
{
	ns4__StopResponse stpresp;
	this->Stop(iBench, stpresp);
	int err;
	ns4__GetStatusResponse gsresp;
	do
	{
		if (this->GetStatus(gsresp)) return -1;

		if (!gsresp.return_)
		{
			LOG_ERR(GetStatus, gsresp.return_->CanceledEvent, "The Bench parameter is not valid!");
			return -1;
		}
		Sleep(1000);

		if (ns3__TStatusCode__stStopTestWait == gsresp.return_->StatusCode)
		{
			ns4__ContinueResponse cresponse;
			err = this->Continue(iBench, cresponse);
			if (SOAP_OK != err)
			{
				LOG_SOAP_ERR(Continue, err);
				return -1;
			}
		}
	}while ((ns3__TStatusCode__stPrepare == gsresp.return_->StatusCode)||\
				(ns3__TStatusCode__stTest == gsresp.return_->StatusCode)||\
				(ns3__TStatusCode__stStartTestWait == gsresp.return_->StatusCode));

	LOG(stopTest, "Test " << gsresp.return_->TestName << " stopped!");
	return -1;
}


#define LINE "Line"

int Bench::setLine(const char* BenchLine)
{
	std::string linestr(LINE);
	if (!std::strcmp("A", BenchLine))
	{
		linestr +="A";
	}
	else if (!std::strcmp("B", BenchLine))
	{
		linestr += "B";
	}
	else
	{
		LOG_ERR(setLine, "Wrong line name!");
		return -1;
	}
	
	std::ofstream cfgcrt;
	cfgcrt.open(CFGFLENME);
	if (cfgcrt.is_open())
	{
		std::string linestr;
		cfgcrt << linestr;
	}
	else
	{	
		LOG_ERR(setLine, "Try to write to cfg file is failed!");
		return -1;
	}
}

const char* errnameret(enum ns3__TResult err)
{
	switch (err)
	{
	case ns3__TResult__rsOK:
		return "Function done succesfully";
		break;
	case ns3__TResult__rsPositionOutOfRange:
		return "Position parameter is not in range from 1 to number of positions on the bench";
		break;
	case ns3__TResult__rsNotApplicable:
		return "Function is not usable in the curren   t program state";
		break;
	case ns3__TResult__rsTestNotFound:
		return "A test, specified by the TestName parameter, was not found";
		break;
	case ns3__TResult__rsInvalidParameter:
		return "A value in a parameter structure is not valid";
		break;
	case ns3__TResult__rsTestAlreadyExists:
		return "A test, specified by the TestName parameter, already exists";
		break;
	case ns3__TResult__rsTypeNotFound:
		return "A type, specified by the TypeName parameter, was not found";
		break;
	case ns3__TResult__rsBenchNotFound:
		return "The bench parameter is not in range from 1 to number of benches";
		break;
	default:
		return "Unknown error!";
	}
}

