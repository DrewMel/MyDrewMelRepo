#pragma once
#include "Util.h"


#define LOG_BENCH_TNM(FOO, BENCH, TESTNM, STR)	std::cout << TimeStamp() << "    [" << #FOO << ']' <<"[BENCH]";\
												BENCH? std::cout << "[LINE A]" :  std::cout << "[LINE B]:";\
												std::cout << TESTNM << " " << STR
								
#define LOG_BENCH(FOO, BENCH, STR)				std::cout << TimeStamp() << "    [" << #FOO << ']' <<"[BENCH]";\
												BENCH? std::cout << "[LINE A] " :  std::cout << "[LINE B] ";\
												std::cout << STR						 
		
#define LOG_SOAP(FOO, STR)						std::cout << TimeStamp() << "    [" << #FOO << ']' <<"[SOAP]";\
												std::cout << STR