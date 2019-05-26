// Engine.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <string>
#include <vector>

/* Grouping process */
#include <GroupingCore/grouping.h>

/* Files & strings management */
#include <GroupingCore/CSVTools.h>
#include <GroupingCore/XLSXTools.h>
#include <GroupingCore/StringTools.h>

/* Communication mangement */
#include <GroupingCore/Socket.h>
#include <GroupingCore/JSONTools.h>

/* Exception management */
#include <GroupingCore/CommonTools.h>
#include <GroupingCore/ProcessException.h>
#include <GroupingCore/ErrorLoger.h>

using namespace std;

string file;
string module = "[Snow Process Helper Engine]";
FileTools::FileManager* file_manager;
Socket* engineSock;
JSONTools::JSONLogCommand *log_msg = new JSONTools::JSONLogCommand();

void set_file(string fullpath){
	file = fullpath;
}

void write_log(JSONTools::JSONCommand* jsonMessage)
{
	ErrorLoger::GetInstance()->OpenStream();

	string strLog = "(" + CommonTools::getStrTime() + ") Info - " + module + " : " + jsonMessage->toJSON() + "\n\n";

	ErrorLoger::GetInstance()->LogLine(strLog.c_str());
	ErrorLoger::GetInstance()->CloseStream();
}

inline void send_log(JSONTools::JSONCommand * jsonMessage)
{
	engineSock->Send(jsonMessage->toJSON().c_str());
	write_log(jsonMessage);
}

void process_grouping(int node_column, int property_column)
{
	string filename = FileTools::FileManager::getFilename(file);
	vector <string> filename_array;
	StringTools::split(filename, '.', filename_array);
	string extension = filename_array[1];

	if (extension.compare(FileTools::FileManager::EXTENSION[FileTools::CSV]) == 0)
		file_manager = new FileTools::CSVTools();
	else
		file_manager = new FileTools::XLSXTools();

	log_msg->setLog(module, "Grouping", JSONTools::LOGINFO, "Reading file");
	send_log(log_msg);

	list<grouping::Node> lno = file_manager->read_file(file, node_column, property_column);

	
	log_msg->setLog(module, "Grouping", JSONTools::LOGINFO, "Tree representation (entities/properties) :");
	send_log(log_msg);

	list<grouping::Node>::iterator it;
	for (it = lno.begin(); it != lno.end(); it++)
	{
		string node = "< " + it->name + " >";

		grouping::Properties::iterator p_it;

		for (p_it = it->properties.begin(); p_it != it->properties.end(); p_it++)
			node += "\n\t" + *p_it;

		log_msg->setLog(module, "Grouping", JSONTools::LOGINFO, node);
		send_log(log_msg);
	}

	//process grouping
	grouping::Group groups = GroupingTools::grouping(lno);
	GroupingTools::show_groups(&groups);//*/
	list<grouping::Node> result = GroupingTools::buildTreeWithGroups(&lno, &groups);

	//save the results
	log_msg->setLog(module, "Grouping", JSONTools::LOGINFO, "Result saving to "+FileTools::FileManager::m_output_fileName);
	send_log(log_msg);
	file_manager->write_file(&result, &groups);
}

int main(int argc, char** argv)
{
	int port = atoi(argv[1]);
	
	engineSock = new Socket(module.c_str(), "127.0.0.1", port);

	if (!engineSock->connection())
		throw ProcessException(ProcessException::FATAL, ProcessException::CONNECTION_FAILED, "Tentative de connexion 127.0.0.1:"+to_string(port));

	JSONTools::JSONConnectionCommand jsonConnect(module);

	send_log((JSONTools::JSONCommand*)&jsonConnect);

	string text = "";
	bool connected = true;

	while (connected)
	{
		text = engineSock->receive();

		log_msg->setLog(module, "Processing", JSONTools::LOGINFO, "Message received : \n" + text);
		write_log(log_msg);

		if (text.compare("") == 0)
			break;

		JSONTools::MESSAGE_TYPE mt;

		try {
			mt = JSONTools::string2command(JSONTools::getStrValueFromJSON(text, "messageType")); 
		}
		catch (const ProcessException& e) {
			e.what();
		}

		switch (mt)
		{
		case JSONTools::EXIT:
			connected = false;
			break;
		case JSONTools::PROCESS:
		{
			JSONTools::JSONProcessAction jsonProcess(text);

			log_msg->setLog(module, "Processing", JSONTools::LOGINFO, "JSON parsing OK");
			write_log(log_msg);

			if (jsonProcess.m_action == JSONTools::GROUPING)
			{
				GroupingTools::connectToUI();

				log_msg->setLog(module, "Grouping", JSONTools::LOGINFO, "Grouping process started");
				send_log(log_msg);

				string ncn = JSONTools::json2string(jsonProcess.getOption("nodeColName"));
				string pcn = JSONTools::json2string(jsonProcess.getOption("propertyColName"));
				int nc = JSONTools::json2int(jsonProcess.getOption("nodeColumn"));
				int pc = JSONTools::json2int(jsonProcess.getOption("propertyColumn"));

				set_file(JSONTools::getStrValueFromJSON(text, "file"));
				FileTools::FileManager::setColumnName(ncn, pcn);

				process_grouping(nc, pc);

				GroupingTools::closeConnection();

				log_msg->setLog(module, "Grouping", JSONTools::LOGINFO, "Grouping process finished");
				send_log(log_msg);
			}

			break;
		}
		default:
			break;
		}
		text = "";
	}
	engineSock->close();
	//system("PAUSE");
}