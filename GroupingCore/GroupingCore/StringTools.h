#ifndef STRINGTOOLS_H
#define STRINGTOOLS_H

#include "framework.h"
#include <iostream>
#include <vector>


class StringTools
{
public:
	StringTools() {};
	static void split(const std::string& s, char delim,std::vector<std::string>& v);
	static int getKeyFromVector(std::vector<std::string> vector, std::string value);
	static void eraseChar(std::string& s, char eraseChar);
	static std::string toLower(std::string str);
	static std::string toUpper(std::string str);
};

#endif