#pragma once

/*
tbench -h															�������� ��� �������

tbench -l <line>													������� ����� �� ���������, ������� ������ ���� ������������ ��� ���������� ������ (A ��� B)

tbench -p															�������� ��� ����� � ��� ������������ ��� ��� ����� � �� ��������� � ���� �������,
																	� ����� �����, ������� ����� ���������, ����� �������� �������� ���������������� ��������� �����.

						MinFlowrate, m3/h    MaxFlowrate, m3/h    MinTemperature, C    MaxTemperature, C    MinVolume, dm3    ProofMethod
	key of parameter		-minf		-maxf			-mint		   -maxt		-vol		-pm
	testname1			...			...		...			...		...		...
	testname2
		.
		.
	testnameN

tbench <testname> -minf <value> -maxf <value> ... -pm <method>	�������� ��������� ������� ��������� ����� testname. ���� ������ ����� ����� ��� ������� �����
																	�� ���������� � ������� ��� ����������� ��������� - ������� ����� ����.

tbench -e <testname>												��������� ���� <testname> ��� ������� ����� � �������� ���������. ���� ��� ������, ��������� ��� ����� � ��� �������,
																	� ������� ��� ������� � �������, �������� ����������.

tbench -s															���������� ���� �� ������� �����

tbench -r <testname> 												������� ���� c ������ testname �� ������� �����

tbench -r															������� ��� ����� ��� ������� �����

ctrl+c																���������� ���� ��� ���� �����
*/
#define HLP "-h"
#define LNE "-l"
#define PRR "-p"
#define EXE "-e"
#define STP "-s"
#define RMV "-r"

#define MinFCOLUMNSTR		"MinFlowrate, m3/h"
#define KEYMinFCOLUMNSTR				"-minf"

#define MaxFCOLUMNSTR		"MaxFlowrate, m3/h"
#define KEYMaxFCOLUMNSTR				"-maxf"

#define MinTCOLUMNSTR		"MinTemperature, C"
#define KEYMinTCOLUMNSTR			"-mint"

#define	MaxTCOLUMNSTR		"MaxTemperature, C"
#define	KEYMaxTCOLUMNSTR				"-maxt"

#define MinVCOLUMNSTR		"MinVolume, dm3"
#define KEYMinVCOLUMNSTR			"-vol"

#define ProofMCOLUMNSTR		"ProofMethod"
#define KEYProofMCOLUMNSTR			"-pm"

#define PMETHODSTR1			"pmFlyGra"
#define PMETHODSTR2			"pmFlyMID"
#define PMETHODSTR3			"pmStdGra"
#define PMETHODSTR4			"pmStdMid"

#define FirstRow			MinFCOLUMNSTR TAB MaxFCOLUMNSTR TAB MinTCOLUMNSTR TAB MaxTCOLUMNSTR TAB MinVCOLUMNSTR TAB ProofMCOLUMNSTR

#define KPNAME				"keyname of parameter"


#define FLOWRATESTR			"Flowrate"
#define WEIGHTSTR			"Weght"
#define INLETTEMPSTR		"InletTemp"
#define OUTLETTEMPSTR		"OutletTemp"
#define INLETPRESSURESTR	"InletPressure"
#define OUTLETPRESSURESTR	"OutletPressure"
#define LAMBDASTR			"Lambda"
#define MIDTEMPSTR			"MIDTemp"
#define MIDPULSECONSTSTR	"MIDPulseConst"
#define MIDVOLUMESTR		"MIDVolume"
#define MIDERRORSTR			"MIDError"
#define DURATIONSTR			"Lambda"
#define ADDITIVELAMDASTR	"AdditiveLambda"

#define SHHLP std::cout << std::endl \
			<< std::endl << "tbench "<<HLP<<"			demonstrate this help"  \
			<< std::endl \
			<< std::endl << "tbench "<<LNE<<" <line>		select bench line for testing (A or B)" \
			<< std::endl \
			<< std::endl << "tbench "<<PRR<<"			show line name and all values of parameters in tabular form and keys raw" \
			<< std::endl << "				 for changing parameters values of existing tests for the line " \
			<< std::endl \
			<< std::endl << "          MinFlowrate,m3/h  MaxFlowrate,m3/h  MinTemperature,C  MaxTemperature,C MinVolume,dm3  ProofMethod" \
			<< std::endl << "   key of       "<<KEYMinFCOLUMNSTR<<"	        "<<KEYMaxFCOLUMNSTR<<"	           "<<KEYMinTCOLUMNSTR<<"	     "<<KEYMaxTCOLUMNSTR<<"          "<<KEYMinVCOLUMNSTR<<"            "<<KEYProofMCOLUMNSTR \
			<< std::endl <<	" parameter"\
			<< std::endl << " testname1       ...             ...                ...                ...            ...            ..." \
			<< std::endl << " testname2" \
			<< std::endl << "    ." \
			<< std::endl << "    ." \
			<< std::endl << " testnameN" \
			<< std::endl \
			<< std::endl << "tbench "<<PRR<<" <testname> "<<KEYMinFCOLUMNSTR<<" <value> "<<KEYMaxFCOLUMNSTR<<" <value> ... "<<KEYProofMCOLUMNSTR<<" <method> "<<" change parameters of test <testname> by" \
			<< std::endl <<"                                 listed keys. If the test do not exist and all amount of parameters are " \
			<< std::endl <<"                                 present, create new test with the parameters" \
			<< std::endl \
			<< std::endl << "tbench "<<EXE<<" <testname>            execute the test <testname> on the current line and display result. If the" \
			<< std::endl << "                                tastename is empty, perform all of the tests for the acting line.The tests " \
			<< std::endl << "                                are performed in the same order they listed." \
			<< std::endl \
			<< std::endl << "tbench "<<STP<<"                       terminate performing test" \
			<< std::endl \
			<< std::endl << "tbench "<<RMV<<" <testname>            remove test with the same testname" \
			<< std::endl \
			<< std::endl << "tbench "<<RMV<<"                       remove all of the listed test for the current line" \
			<< std::endl \
			<< std::endl << "ctrl+c                          terminate test performing" \
			<< std::endl \
			<< std::endl; \
