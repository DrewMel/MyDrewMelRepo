#pragma once
#include "IJusWTBServicebinding.nsmap"
#include "stdsoap2.h"
#include "soapIJusWTBServicebindingProxy.h"
#include <string>

class BenchTest
{
public:
	std::string testName;
	ns3__TTestParameters parameters;
	ns3__TTestResult result;
};