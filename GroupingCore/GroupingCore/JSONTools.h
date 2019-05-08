#pragma once
#include "framework.h"

#include <iostream>
#include "ProcessException.h"
#include <vector>

/* JSON includes */
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

namespace JSONTools 
{
	enum MESSAGE_TYPE { PROCESS_FILE, CONNECTION, LOG, UNDEFINED };

	enum LOG_LEVEL { LOGINFO = 0, LOGERROR = 1 };
	const std::string LOG_LEVEL_STR[] = {"Information", "Error"};


	/* Tools to build JSON messages */

	struct JSONCommand {
		std::string m_message_type;
		std::string m_client;
		virtual std::string toJSON() = 0;
	};

	struct JSONConnectionCommand : JSONCommand
	{
		std::string m_message = "connected";

		JSONConnectionCommand(std::string client){
			m_message_type = "connection";
			m_client = client;
		}

		std::string toJSON() {
			rapidjson::StringBuffer jsonMsg;
			rapidjson::Writer<rapidjson::StringBuffer> writer(jsonMsg);

			writer.StartObject();
			writer.Key("messageType");
			writer.String(m_message_type.c_str());
			writer.Key("client");
			writer.String(m_client.c_str());
			writer.Key("message");
			writer.String(m_message.c_str());
			writer.EndObject();

			return jsonMsg.GetString();
		}
	};

	struct JSONLogCommand : JSONCommand
	{
		std::string m_operation;
		LOG_LEVEL m_level;
		std::string m_log;
		std::string m_comment;

		void setLog(std::string client, std::string operation, LOG_LEVEL level, std::string log, std::string comment = ""){
			m_message_type = "log";
			m_client = client;
			m_operation = operation;
			m_level = level;
			m_log = log;
			m_comment = comment;
		}

		std::string toJSON() {
			rapidjson::StringBuffer jsonMsg;
			rapidjson::Writer<rapidjson::StringBuffer> writer(jsonMsg);

			writer.StartObject();
			writer.Key("messageType");
			writer.String(m_message_type.c_str());
			writer.Key("client");
			writer.String(m_client.c_str());
			writer.Key("operation");
			writer.String(m_operation.c_str());
			writer.Key("level");
			writer.String(LOG_LEVEL_STR[m_level].c_str());
			writer.Key("log");
			writer.String(m_log.c_str());
			writer.Key("comment");
			writer.EndObject();

			return jsonMsg.GetString();
		}
	};


	/* Tools to read JSON messages */

	std::string getStrValueFromJSON(std::string msg, std::string key)
	{
		rapidjson::Document d;
		d.Parse(msg.c_str());
		std::string trace = "get string member " + key + " from JSON object " + msg;

		if (!d.HasMember(key.c_str()))
			throw ProcessException(ProcessException::FATAL, ProcessException::NON_EXISTENT, trace.c_str());

		rapidjson::Value& s = d[key.c_str()];

		if (!s.IsString())
		{
			try {
				throw ProcessException(ProcessException::INCIDENT, ProcessException::UNEXPECTED_VALUE, trace.c_str());
			}
			catch (ProcessException const& e) {
				e.what();
				return "";
			}
		}

		return s.GetString();
	}

	int getIntValueFromJSON(std::string msg, std::string key)
	{
		rapidjson::Document d;
		d.Parse(msg.c_str());
		std::string trace = "get integer member " + key + " from JSON object " + msg;

		if (!d.HasMember(key.c_str()))
			throw ProcessException(ProcessException::FATAL, ProcessException::NON_EXISTENT, trace.c_str());

		rapidjson::Value& s = d[key.c_str()];

		if (!s.IsInt())
		{
			try {
				throw ProcessException(ProcessException::INCIDENT, ProcessException::UNEXPECTED_VALUE, trace.c_str());
			}
			catch (ProcessException const& e) {
				e.what();
				return 0;
			}
		}

		return s.GetInt();
	}

	bool getBoolValueFromJSON(std::string msg, std::string key)
	{
		rapidjson::Document d;
		d.Parse(msg.c_str());
		std::string trace = "get bool member " + key + " from JSON object " + msg;

		if (!d.HasMember(key.c_str()))
			throw ProcessException(ProcessException::FATAL, ProcessException::NON_EXISTENT, trace.c_str());

		rapidjson::Value& s = d[key.c_str()];

		if (!s.IsBool())
		{
			try {
				throw ProcessException(ProcessException::INCIDENT, ProcessException::UNEXPECTED_VALUE, trace.c_str());
			}
			catch (ProcessException const& e) {
				e.what();
				return false;
			}
		}

		return s.GetBool();
	}

	std::vector<std::string> getArrayValueFromJSON(std::string msg, std::string key)
	{
		rapidjson::Document d;
		d.Parse(msg.c_str());
		std::vector<std::string> values;
		std::string trace = "get string array member " + key + " from JSON object " + msg;

		if (!d.HasMember(key.c_str()))
			throw ProcessException(ProcessException::FATAL, ProcessException::NON_EXISTENT, trace.c_str());

		rapidjson::Value& s = d[key.c_str()];

		if (!s.IsArray())
		{
			try {
				throw ProcessException(ProcessException::INCIDENT, ProcessException::UNEXPECTED_VALUE, trace.c_str());
			}
			catch (ProcessException const& e) {
				e.what();
				return values;
			}
		}

		for (rapidjson::SizeType i = 0; i < s.Size(); i++) // Uses SizeType instead of size_t
			values.push_back(s[i].GetString());

		return values;
	}

	MESSAGE_TYPE string2command(std::string cmd)
	{
		if (cmd.compare("process") == 0)
			return PROCESS_FILE;
		else if (cmd.compare("log") == 0)
			return LOG;
		else if (cmd.compare("connection") == 0)
			return CONNECTION;
		else
		{
			try {
				throw ProcessException(ProcessException::INCIDENT, ProcessException::UNEXPECTED_MESSAGE);
			}
			catch (ProcessException const& e) {
				e.what();
				return UNDEFINED;
			}
		}
	}
}