#pragma once

#include "framework.h"

#include <exception>
#include <string>
#include <iostream>
#include <map>
#include <utility>

typedef std::pair<std::string, std::string> Information; //<name, description>

class GROUPINGCORE_API ProcessException : public std::exception
{
public:
	enum ERROR_LEVEL { INCIDENT, FATAL };
	enum ERROR_TYPE
	{
		UNDEFINED = 0,
		NON_EXISTENT = 1,
		UNEXPECTED_VALUE = 2,
		UNEXPECTED_MESSAGE = 3,
		FILE_OPEN = 4
	};

	ProcessException(ERROR_LEVEL level, ERROR_TYPE type, const std::string trigger = "") throw();
	~ProcessException() throw();

	virtual const char* what() const throw();

	const std::string getLevel() const throw();
	const std::string getTrigger() const throw();
	const Information getErrorInfo() const throw();

private:
	ERROR_LEVEL m_level;
	ERROR_TYPE m_type;
	std::string m_name;
	std::string m_description;
	std::string m_trigger;

	/* error's names and descriptions */
	static const std::string ERROR_NAME[];
	static const std::string ERROR_DESCRIPTION[];
};

