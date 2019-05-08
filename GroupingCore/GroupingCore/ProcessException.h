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
	enum ERROR_LEVEL { INCIDENT, FATAL};
	enum ERROR_TYPE
	{
		NON_EXISTENT,
		UNEXPECTED_VALUE,
		UNEXPECTED_MESSAGE,
		UNDEFINED
	};

	ProcessException(ERROR_LEVEL level, ERROR_TYPE type, const char * trigger = NULL) throw();
	~ProcessException() throw();

	virtual const char* what() const throw();

	const std::string getLevel() const throw();
	const std::string getTrigger() const throw();
	

private:
	ERROR_LEVEL m_level;
	std::string m_name;
	std::string m_description;
	const char* m_trigger;
	std::map<ERROR_TYPE, Information > m_error;

	/* error's names */
	static const std::string STR_NON_EXISTENT;
	static const std::string STR_UNEXPECTED_VALUE;
	static const std::string STR_UNEXPECTED_MESSAGE;
	static const std::string STR_UNDEFINED;

	/* error's descriptions */
	static const std::string DESC_NON_EXISTENT;
	static const std::string DESC_UNEXPECTED_VALUE;
	static const std::string DESC_UNEXPECTED_MESSAGE;
	static const std::string DESC_UNDEFINED;

	const Information getErrorInfo(ERROR_TYPE errType) const throw();
	void setError(ERROR_TYPE errType, std::string& name, std::string& description) throw();
};

