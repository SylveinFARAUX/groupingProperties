#include "ErrorLoger.h"
#include "CommonTools.h"
#include "StringTools.h"
#include <iostream>


const char * ErrorLoger::fileName = "SnowProcess Helper.log";

bool ErrorLoger::checkDate()
{
	bool append = false;
	std::ios_base::openmode openMode = std::ios_base::in;

	std::ifstream	inputFile(fileName, openMode);
		
	if(!inputFile.bad())
	{
		std::string header;
		int fileDay;

		// check the date
		const int myDay = CommonTools::getFullDayNumber();

		//check the file date
		std::getline(inputFile, header);

		if (header.length() < 3)
			return append;

		header = header.substr(0,3);
		StringTools::trim(header);

		//do stuff
		fileDay = std::stoi(header);

		if (fileDay == myDay)
			append = true;

		// close the file
		inputFile.close();
	}

	return append;
}

void ErrorLoger::OpenStream()
{
	bool append = checkDate();

	std::ios_base::openmode openMode = std::ios_base::out;

	if(append)
		openMode |= std::ios_base::app;
	else
		openMode |= std::ios_base::trunc;

	m_fileStream.open(fileName, openMode);

	if(m_fileStream.bad())
	{
		std::cerr << "ErrorLoger::OpenStream() Error opening stream" << std::endl;
	}
	else
	{
		if (!append)
		{
			std::string header = std::to_string(CommonTools::getFullDayNumber()) + "  -  File created at " + CommonTools::getFullTimeStr() + "\n\n";
			LogLine(header.c_str());
		}
	}
}

void ErrorLoger::LogLine(std::string line)
{
	if(m_fileStream.is_open())
	{
		//std::cout << "ErrorLoger::LogLine() " << line.c_str() << std::endl;
		const char* charArray = line.c_str();
		unsigned long size = line.length() * sizeof(char);
		m_fileStream.write(charArray, size);
		m_fileStream.flush();
	}
}

void ErrorLoger::CloseStream()
{
	if(m_fileStream.is_open())
	{
		m_fileStream.close();
	}
}