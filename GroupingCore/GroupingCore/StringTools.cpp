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