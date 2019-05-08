#ifndef STRINGTOOLS_H
#define STRINGTOOLS_H

#include "framework.h"
#include <iostream>
#include <vector>


class GROUPINGCORE_API StringTools
{
public:
	/* General tools */
	static void split(const std::string& s, char delim,std::vector<std::string>& v);
	static int getKeyFromVector(std::vector<std::string> vector, std::string value);
	static void eraseChar(std::string& s, char eraseChar);
	static std::string toLower(std::string str);
	static std::string toUpper(std::string str);

	/* JSON tools */

	/* Conversion tools */
	static wchar_t* string2wchar(std::string str);
	static std::string wchar2string(const wchar_t* wide_string);
};

#endif