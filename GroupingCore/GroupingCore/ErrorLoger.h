#pragma once

#include "framework.h"
#include <fstream>

class GROUPINGCORE_API ErrorLoger
{
protected:
	bool checkDate();

	std::ofstream	m_fileStream;
	static const char* fileName;
	static ErrorLoger* m_instance;

public:
	void OpenStream();
	void LogLine(std::string line);
	void CloseStream();

	static ErrorLoger* GetInstance()
	{
		if (m_instance == NULL)
			m_instance = new ErrorLoger();

		return m_instance;
	}
};