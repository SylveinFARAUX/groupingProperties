#include "ProcessException.h"

/* Definition of errors' names and descriptions */
const std::string ProcessException::STR_NON_EXISTENT = "Non-existent Variable";
const std::string ProcessException::STR_UNEXPECTED_VALUE = "Unexpected Value";
const std::string ProcessException::STR_UNEXPECTED_MESSAGE = "Unexpected Message";
const std::string ProcessException::STR_UNDEFINED = "Undefined Error";

const std::string ProcessException::DESC_NON_EXISTENT = "The variable you are looking for does not exist.";
const std::string ProcessException::DESC_UNEXPECTED_VALUE = "The returned type is not as expected, please check the JSON message.";
const std::string ProcessException::DESC_UNEXPECTED_MESSAGE = "The message content is unexpected, please verify the values.";
const std::string ProcessException::DESC_UNDEFINED = "Undefined error.";

ProcessException::ProcessException(ERROR_LEVEL level, ERROR_TYPE type, const char* trigger)
{
	setError(type, m_name, m_description);
	m_level = level;
	m_trigger = trigger;
}

ProcessException::~ProcessException(){
	m_trigger = NULL;
}

void ProcessException::setError(ERROR_TYPE errType, std::string& name, std::string& description)
{
	m_error[NON_EXISTENT] = Information(STR_NON_EXISTENT, DESC_NON_EXISTENT);
	m_error[UNEXPECTED_VALUE] = Information(STR_UNEXPECTED_VALUE, DESC_UNEXPECTED_VALUE);
	m_error[UNEXPECTED_MESSAGE] = Information(STR_UNEXPECTED_MESSAGE, DESC_UNEXPECTED_MESSAGE);
	m_error[UNDEFINED] = Information(STR_UNDEFINED, DESC_UNDEFINED);

	Information info = getErrorInfo(errType);
	name = info.first;
	description = info.second;
}

const char* ProcessException::what() const throw()
{
	std::string strError = (getTrigger().length() <= 0) ? getLevel() + " " + m_name + " : " + m_description + "\n"
														: getLevel() + " " + m_name + " : " + m_description + " Triggered by :\n" + getTrigger() + "\n";
	std::cerr << strError;

	// TODO log this error with time

	return strError.c_str();
}

const std::string ProcessException::getLevel() const{
	return (m_level == INCIDENT) ? "Error" : "Fatal Error";
}

const std::string ProcessException::getTrigger() const{
	return (m_trigger != NULL) ? std::string(m_trigger) : "";
}

const Information ProcessException::getErrorInfo(ERROR_TYPE errType) const{
	return (m_error.find(errType) != m_error.end()) ? m_error.at(errType) : m_error.at(UNDEFINED);
}
