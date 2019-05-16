#include "StringTools.h"

using namespace std;

void StringTools::split(const std::string& s, char delim,std::vector<std::string>& v) {
	auto i = 0;
	auto pos = s.find(delim);
	while (pos != std::string::npos) {
		v.push_back(s.substr(i, pos-i));
		i = ++pos;
		pos = s.find(delim, pos);

		if (pos == std::string::npos)
			v.push_back(s.substr(i, s.length()));
	}
}

int StringTools::getKeyFromVector(std::vector<std::string> vector, std::string value)
{
	int indice =-1;

	for(unsigned int i = 0 ; i < vector.size() ; i++)
		if (vector.at(i).compare(value) == 0){
			indice = i;
			break;
		}

	return indice;
}

void StringTools::eraseChar(string& s, char eraseChar) {
	s.erase(remove(s.begin(), s.end(), eraseChar), s.end());
}

string StringTools::toLower(string str){
	string lowerStr = string(str);
	std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
	return lowerStr;
}

string StringTools::toUpper(string str){
	string upperStr = string(str);
	std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
	return upperStr;
}

wchar_t* StringTools::string2wchar(string str)
{
	wchar_t* wide_string = new wchar_t[str.length() + 1];
	std::copy(str.begin(), str.end(), wide_string);
	wide_string[str.length()] = 0;

	return wide_string;
}

string StringTools::wchar2string(const wchar_t* wide_string)
{
	wstring ws(wide_string);
	return string(ws.begin(), ws.end());
}

inline void StringTools::ltrim(string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
		}));
}

inline void StringTools::rtrim(string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
		}).base(), s.end());
}

inline void StringTools::trim(string& s) {
	ltrim(s);
	rtrim(s);
}

bool StringTools::isValidString(string* val)
{
	const char* const_val = val->c_str();

	for (int i = 0; i < val->length(); i++)
	{
		int code = int(const_val[i]);

		if (!((65 <= code && code <= 90) || (97 <= code && code <= 122) || code == 95 || code == 46 || (48 <= code && code <= 57)))
			return false;
	}

	return true;
}