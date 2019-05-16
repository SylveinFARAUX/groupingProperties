#pragma once

#include "Singleton.h"
#include <fstream>

class GROUPINGCORE_API ErrorLoger : public Singleton<ErrorLoger>
{
public:
	void OpenStream();
	void LogLine(std::string line);
	void CloseStream();

protected:
	bool checkDate();

	std::ofstream	m_fileStream;		
	static const char * fileName;
};