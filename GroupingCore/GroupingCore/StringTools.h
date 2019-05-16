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
	static inline void trim(std::string& s); // trim from both ends (in place)
	bool isValidString(std::string* val);


	/* Conversion tools */
	static wchar_t* string2wchar(std::string str);
	static std::string wchar2string(const wchar_t* wide_string);

private:
	// trim from start (in place)
	static inline void ltrim(std::string& s);

	// trim from end (in place)
	static inline void rtrim(std::string& s);
};

#endif