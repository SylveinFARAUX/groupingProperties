#include "ProcessException.h"
#include "CommonTools.h"
#include "ErrorLoger.h"

/* Definition of errors' names and descriptions */
const std::string ProcessException::ERROR_NAME[] = {
	"Undefined Error",
	"Non-existent Variable",
	"Unexpected Value",
	"Unexpected Message",
	"File Opening",
	"Connection Failed"
};

const std::string ProcessException::ERROR_DESCRIPTION[] = {
	"Undefined error.",
	"The variable you are looking for does not exist.",
	"The returned type is not as expected, please check the JSON message.",
	"The message content is unexpected, please verify the values.",
	"Unable to open the file, please verify your rights to open it.",
	"Unable to etablish the connection."
};


ProcessException::ProcessException(ERROR_LEVEL level, ERROR_TYPE type, const std::string trigger)
{
	m_type = type;
	m_name = ERROR_NAME[type];
	m_description = ERROR_DESCRIPTION[type];
	m_level = level;
	m_trigger = trigger;
	m_time = CommonTools::getTime();
}

ProcessException::~ProcessException(){}

const char* ProcessException::what() const throw()
{
	std::string strLogLine;
	ErrorLoger::GetInstance()->OpenStream();
	std::string strError = (m_trigger.length() <= 0) ? getLevel() + " " + m_name + " : " + m_description + "\n"
														: getLevel() + " " + m_name + " : " + m_description + " Triggered by :\n" + m_trigger + "\n";
	std::cerr << strError;

	//then log this error with time
	strLogLine = (getTrigger().length() > 0) ? "(" + CommonTools::getStrTimeFromStruct(m_time) + ") " + getLevel() + " - " + m_name + " : " + m_description + " Message ignored :\n" + getTrigger() + "\n\n" : "(" + CommonTools::getStrTimeFromStruct(m_time) + ") " + getLevel() + " - " + m_name + " : " + m_description + "\n\n";
	ErrorLoger::GetInstance()->LogLine(strLogLine.c_str());

	ErrorLoger::GetInstance()->CloseStream();

	return strError.c_str();
}

const std::string ProcessException::getLevel() const{
	return (m_level == INCIDENT) ? "Error" : "Fatal Error";
}

const std::string ProcessException::getTrigger() const{
	return m_trigger;
}

const Information ProcessException::getErrorInfo() const{
	return Information(ERROR_NAME[m_type], ERROR_DESCRIPTION[m_type]);
}
