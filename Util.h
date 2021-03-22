#pragma once

#include <ctime>
#include <iostream>
#include <sstream>

#define defnum2text(x) #x
#define D2T(x) defnum2text(x)


inline std::string TimeStamp(void)
{
	std::stringstream ss;
	time_t tme = time(nullptr);
	tm tmstru;
	localtime_s(&tmstru, &tme);
	ss << tmstru.tm_hour << ":" << tmstru.tm_min / 10 << tmstru.tm_min % 10 << ":" << tmstru.tm_sec / 10 << tmstru.tm_sec % 10;
	return ss.str();
}

inline UINT maxintval(UINT a, UINT b)
{
	return a > b ? a : b;
}

#define DOUBLEIS20dec 20
inline bool str2double(const char str[], double& val)
{
	bool fractal = false;
	int fractpwr = 1;
	val = 0;
	int i = 0;
	while (str[i])
	{
		if (i == DOUBLEIS20dec)
		{
			return false;
		}
		if ((str[i] >= '0') && (str[i] <= '9') || (str[i] == '.'))
		{
			if (str[i] == '.')
			{
				if (fractal)
					return false;
				fractal = true;
			}
			else
			{
				if (fractal)
				{
					fractpwr = fractpwr * 10;
				}
				val = val * 10 + (str[i] - '0');
			}
			i++;
		}
		else
		{
			return false;
		}
	}
	val = val / fractpwr;
	return true;
}


class spc
{
private:
	char* space;
public:
	spc():space(nullptr)
	{

	}
	~spc()
	{
		delete[] space;
	}
	char* getnspcs(int n)
	{
		if (nullptr == space)
		{
			delete[] space;
		}
		space = new char[n];
		for (int i = 0; i < n; i++)
		{
			space[i] = '2';
		}
		space[n - 1] = 0;
		return space;
	}
};