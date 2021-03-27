#include <iostream>
#include <string>
#include "IJusWTBServicebinding.h"
#include "Bench.h"
#include "strs.h"

namespace Mainkeys {
	enum {
		HELP,
		LINE,
		SETPARAMETER,
		EXECUTE,
		STOP,
		REMOVE,
		MAIN_KEYS_NUM
	};
};

namespace Pmtrtbl {
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
};

namespace Rslttbl {

	enum { TNCOLUMN, FLOWRATE, WEIGHT, INLETTEMP, OUTLETTEMP, INLETPRESSURE, OUTLETPRESSURE, LAMBDA, MIDTEMP, MIDPULSECONST, MIDVOLUME, MIDERROR, DURATION, ADDITIVELAMDA, COLUMN_NUM };
};

enum Purpse {NOPARAMETER = 0, TEXT, VALUE, MANDATORYTEXT, MANDATORYVALUE};

struct dictword { std::string keynm; Purpse prps; const dictword* sk; };

const dictword ParamDictionary[]
{
	{KEYMinFCOLUMNSTR, MANDATORYVALUE, nullptr},
	{KEYMaxFCOLUMNSTR, MANDATORYVALUE, nullptr},
	{KEYMinTCOLUMNSTR, MANDATORYVALUE, nullptr},
	{KEYMaxTCOLUMNSTR, MANDATORYVALUE, nullptr},
	{KEYMinVCOLUMNSTR, MANDATORYVALUE, nullptr},
	{KEYProofMCOLUMNSTR, MANDATORYVALUE, nullptr},
	{nullptr, NOPARAMETER, nullptr}
};

const dictword Dictionary[] =
{
	{HLP, NOPARAMETER, nullptr},
	{LNE, MANDATORYTEXT, nullptr},
	{PRR, TEXT, ParamDictionary},
	{EXE, TEXT, nullptr},
	{STP, NOPARAMETER, nullptr},
	{RMV, TEXT, nullptr},
	{nullptr, NOPARAMETER, nullptr}
};

  
class key {
public:
	int priority;
	char * parameter;
	key* subkey;
	int num;
	key* tempkyarr;

	int addsubkey(int priority, char* parameter)
	{
		tempkyarr = new key[1+num];
		for (int i = 0; i < num; i++)
		{
			if (priority == subkey->priority)
			{
				return -1;
			}
			if (priority > subkey[i].priority)
			{
				tempkyarr[i].parameter = parameter;
				tempkyarr[i].priority = priority;
				tempkyarr[i + 1] = subkey[i];
			}
			else
			{
				tempkyarr[i] = subkey[i];
			}		
		}
		delete[] subkey;
		subkey = tempkyarr;
	}

	~key(void)
	{
		delete[] tempkyarr;
	}
};


int argpars(const dictword Dictionary[], const int args, const char* argc[], key* subkey)
{
	for (int i = 0; i < args; i++)
	{
		int j = 0;
		while (nullptr != Dictionary[j++].keynm.c_str())
		{
			if (!Dictionary[j].keynm.compare(argc[i]))
			{
				subkey->addsubkey(j, );
			}
		}	
	}
}

#define DICTSIZE sizeof(Dictionary)/sizeof(Dictionary[0])
#define PRMDICTSIZE sizeof(ParamDictionary)/sizeof(ParamDictionary[0])
#define STRPROGNAME 0
#define APPPATH		0
int main(const int args, const char* argc[])
{
	if (args == 1)
	{
		std::cout << "Error: No arguments, use -h to show help" << std::endl;
		return -1;
	}
	for (int i = APPPATH + 1; i < args; i++)
	{
		if ('-' == argc[i][0])
		{
			for (int j = 0; j < DICTSIZE; j++)
			{
				if (Dictionary[j].keynm.compare(argc[i]))
				{
					key tt;
					tt.priority = j;
					tt.parameter = ;
				}
				else
				{
					std::cout << "Error: wrong key!" << std::endl;
					return -1;
				}
			}
		}
		else if (APPPATH + 1 == i)
		{
			std::cout << "Error: the first argument should be key!" << std::endl;
			return -1;
		}
	}



	struct substr {
		bool used;
		int wordnum;
		const char** ptrsubstr;
	}keyssubstr[Mainkeys::DICT_SIZE] = {false, 0};

	int prevsubstr = 0;
	for (int i = STRPROGNAME+1; i < args; i++)
	{
		for (int j = 0; j < Mainkeys::DICT_SIZE; j++)
		{
			if (Dictionary[j].compare(argc[i]));
			{
				if (keyssubstr[j].used)
				{
					std::cout << "The key " << Dictionary[j] << " is repeating!" << std::endl;
					return -1;
				}
				else
				{
					keyssubstr[j].used = true;
					keyssubstr[j].wordnum = i - prevsubstr - 1;
					keyssubstr[j].ptrsubstr = (argc + j);
					prevsubstr = i;
					break;
				}
			}
		}
	}
	Bench Bench;
	for (int i = 0; i < Mainkeys::DICT_SIZE; i++)
	{
		
		if (keyssubstr[i].used)
		{
			switch (i)
			{
			case Mainkeys::HELP:
				SHHLP;
				break;
			case Mainkeys::LINE:
				if (keyssubstr[i].wordnum == 1)
				{
					Bench.setLine(*keyssubstr[i].ptrsubstr);
				}
				else
				{
					return -1;
				}
					
				break;
			case Mainkeys::SETPARAMETER:
				if (keyssubstr[i].wordnum > 1 + Pmtrtbl::COLUMN_NUM * 2)
				{
					std::cout << "Too many arguments!" << std::endl;
					return -1;
				}
				if (keyssubstr[i].wordnum > 1 + 1 + 1)
				{
					std::cout << "Too few arguments!" << std::endl;
					return -1;
				}
				Bench.setTestParameter(keyssubstr[i].wordnum, keyssubstr[i].ptrsubstr);
				break;
			case Mainkeys::EXECUTE:
				if (keyssubstr[i].wordnum == 0)
				{
					Bench.executeTest(nullptr);
				}
				else if(keyssubstr[i].wordnum == 1)
				{
					Bench.executeTest(*keyssubstr[i].ptrsubstr);
				}
				else
				{
					return -1;
				}
				break;
			case Mainkeys::STOP:
				if (keyssubstr[i].wordnum == 0)
				{
					Bench.stopTest();
				}
				else
				{
					return -1;
				}
				break;
			case Mainkeys::REMOVE:
				if (keyssubstr[i].wordnum == 0)
				{
					Bench.executeTest(nullptr);
				}
				else if (keyssubstr[i].wordnum == 1)
				{
					Bench.executeTest(*keyssubstr[i].ptrsubstr);
				}
				else
				{
					return -1;
				}
				break;
			}
		}
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
