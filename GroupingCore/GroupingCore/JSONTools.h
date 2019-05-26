#pragma once
#include "framework.h"

#include <iostream>
#include "ProcessException.h"
#include <vector>
#include <map>
#include <unordered_map>

/* JSON includes */
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>


namespace JSONTools 
{
	typedef std::unordered_map<std::string, const void*> Hashtable;
	enum MESSAGE_TYPE { PROCESS, CONNECTION, LOG, EXIT, MESSAGE_UNDEFINED };
	enum Action { GROUPING, ACTION_UNDEFINED };
	enum LOG_LEVEL { LOGINFO = 0, LOGERROR = 1 };
	const std::string LOG_LEVEL_STR[] = {"Information", "Error"};


	/* Tools to understand TCP messages */

	MESSAGE_TYPE string2command(std::string cmd)
	{
		if (cmd.compare("process") == 0)
			return PROCESS;
		else if (cmd.compare("log") == 0)
			return LOG;
		else if (cmd.compare("connection") == 0)
			return CONNECTION;
		else if (cmd.compare("exit") == 0)
			return EXIT;
		else
		{
			try {
				throw ProcessException(ProcessException::INCIDENT, ProcessException::UNEXPECTED_MESSAGE);
			}
			catch (ProcessException const& e) {
				e.what();
				return MESSAGE_UNDEFINED;
			}
		}
	}

	Action string2Action(std::string cmd)
	{
		if (cmd.compare("grouping") == 0)
			return GROUPING;
		else
			return ACTION_UNDEFINED;
	}


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
	
	rapidjson::GenericObject<false, rapidjson::Value> getObjectValueFromJSON(std::string msg, std::string key)
	{
		rapidjson::Document d;
		d.Parse(msg.c_str());

		std::string trace = "get JSON object member " + key + " from JSON object " + msg;

		if (!d.HasMember(key.c_str()))
			throw ProcessException(ProcessException::FATAL, ProcessException::NON_EXISTENT, trace.c_str());

		rapidjson::Value& s = d[key.c_str()];

		return s.GetObjectW();
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
	
	std::string json2string(const void* val) 
	{ 
		if (!((rapidjson::Value*)val)->IsString())
		{
			try {
				throw ProcessException(ProcessException::INCIDENT, ProcessException::UNEXPECTED_VALUE);
			}
			catch (ProcessException const& e) {
				e.what();
				return "";
			}
		}
		return ((rapidjson::Value*)val)->GetString(); 
	}

	int json2int(const void* val) 
	{ 
		if (!((rapidjson::Value*)val)->IsInt())
		{
			try {
				throw ProcessException(ProcessException::INCIDENT, ProcessException::UNEXPECTED_VALUE);
			}
			catch (ProcessException const& e) {
				e.what();
				return 0;
			}
		}
		return ((rapidjson::Value*)val)->GetInt(); 
	}

	bool json2bool(const void* val) 
	{ 
		if (!((rapidjson::Value*)val)->IsBool())
		{
			try {
				throw ProcessException(ProcessException::INCIDENT, ProcessException::UNEXPECTED_VALUE);
			}
			catch (ProcessException const& e) {
				e.what();
				return "";
			}
		}
		return ((rapidjson::Value*)val)->GetBool(); 
	}


	/* Tools to build JSON messages */

	struct JSONCommand {
		std::string m_message_type;
		std::string m_client;
		virtual std::string toJSON() = 0;
	};

	struct JSONAction {
		std::string m_message_type;
		Hashtable m_option;
	};

	struct JSONConnectionCommand : JSONCommand
	{

		JSONConnectionCommand(std::string client) {
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

		void setLog(std::string client, std::string operation, LOG_LEVEL level, std::string log, std::string comment = "") {
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
			writer.String(m_comment.c_str());
			writer.EndObject();

			return jsonMsg.GetString();
		}
	};

	struct JSONProcessAction : JSONAction
	{
		Action m_action = ACTION_UNDEFINED;
		std::string m_file = "";
		std::string m_comment;
		rapidjson::Document option_doc;

		JSONProcessAction(std::string jsonMessage) {
			m_message_type = "process";

			try {
				m_action = string2Action(getStrValueFromJSON(jsonMessage, "action"));
				m_file = getStrValueFromJSON(jsonMessage, "file");
				option_doc.Parse(jsonMessage.c_str());

				for (rapidjson::Value::ConstMemberIterator mit = option_doc["option"].MemberBegin(); mit != option_doc["option"].MemberEnd(); mit++)
					m_option[mit->name.GetString()] = &mit->value;
			}
			catch (const ProcessException & e) {
				e.what();
			}
		}
		const void* getOption(std::string option) { return m_option[option]; }
	};
}