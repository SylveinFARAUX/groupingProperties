#pragma once

#include "framework.h"
#include <ctime>  
#include <vector>
#include <string>


class GROUPINGCORE_API CommonTools
{
public:
	CommonTools();
	static struct tm* getTime();
	static std::string const getStrTime();
	static std::string const getFullTimeStr();
	static std::string const getStrTimeFromStruct(tm * m_time);
	static int const getFullDayNumber();
	static bool isValidString(std::string * val);
	static bool isExistingName(std::string target, std::vector<std::string> base);

	//remove duplicates from std vector
	template <typename Type>
	static void remove_duplicate(std::vector<Type> &vec) {
		std::sort(vec.begin(), vec.end());
		vec.erase(unique(vec.begin(), vec.end()), vec.end());
	}

	//show its IPv4 address
	static void showIPv4();
	static void showAddr();
};
