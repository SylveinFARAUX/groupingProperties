// Grouping.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <string>
#include <vector>


#include <GroupingCore/grouping.h>

#include <GroupingCore/CSVTools.h>
#include <GroupingCore/XLSXTools.h>
#include <GroupingCore/StringTools.h>

#include <GroupingCore/Socket.h>

/* JSON management */
#include <GroupingCore/JSONTools.h>

/* Exception management*/
#include <GroupingCore/ProcessException.h>

using namespace std;

string file;
string module = "[Snow Process Helper Engine]";
FileTools::FileManager* file_manager;
Socket* engineSock;
JSONTools::JSONLogCommand *log_msg = new JSONTools::JSONLogCommand();

void set_file(string fullpath){
	file = fullpath;
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
	engineSock->Send(log_msg->toJSON().c_str());

	list<grouping::Node> lno = file_manager->read_file(file, node_column, property_column);

	
	log_msg->setLog(module, "Grouping", JSONTools::LOGINFO, "Tree representation (entities/properties) :");
	engineSock->Send(log_msg->toJSON().c_str());

	list<grouping::Node>::iterator it;
	for (it = lno.begin(); it != lno.end(); it++)
	{
		string node = "< " + it->name + " >";

		grouping::Properties::iterator p_it;

		for (p_it = it->properties.begin(); p_it != it->properties.end(); p_it++)
			node += "\n\t" + *p_it;

		log_msg->setLog(module, "Grouping", JSONTools::LOGINFO, node);
		engineSock->Send(log_msg->toJSON().c_str());
	}

	//process grouping
	grouping::Group groups = grouping::GroupingTools::grouping(lno);
	grouping::GroupingTools::show_groups(&groups);//*/
	list<grouping::Node> result = grouping::GroupingTools::buildTreeWithGroups(&lno, &groups);

	//save the results
	log_msg->setLog(module, "Grouping", JSONTools::LOGINFO, "Result saving to "+FileTools::FileManager::m_output_fileName);
	engineSock->Send(log_msg->toJSON().c_str());
	file_manager->write_file(&result, &groups);
}

int main()
{
	Sleep(3000);
	Socket SnowProcessHelperUI = Socket(module.c_str(), "127.0.0.1", 40009);
	engineSock = &SnowProcessHelperUI;

	if (!SnowProcessHelperUI.connection())
		throw ProcessException(ProcessException::FATAL, ProcessException::CONNECTION_FAILED, "Tentative de connexion 127.0.0.1:40000");

	SnowProcessHelperUI.Send(JSONTools::JSONConnectionCommand(module).toJSON().c_str(), true);

	string text = "";
	bool connected = true;

	while (connected)
	{
		
		text = SnowProcessHelperUI.receive();
		cout << "message is : " << text << endl;
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
			cout << "parsing du JSON OK " << endl;

			if (jsonProcess.m_action == JSONTools::GROUPING)
			{
				log_msg->setLog(module, "Grouping", JSONTools::LOGINFO, "Grouping process started");
				SnowProcessHelperUI.Send(log_msg->toJSON().c_str(), true);
				cout << "3" << endl;

				string ncn = JSONTools::json2string(jsonProcess.getOption("nodeColName"));
				string pcn = JSONTools::json2string(jsonProcess.getOption("propertyColName"));
				int nc = JSONTools::json2int(jsonProcess.getOption("nodeColumn"));
				int pc = JSONTools::json2int(jsonProcess.getOption("propertyColumn"));

				set_file(JSONTools::getStrValueFromJSON(text, "file"));
				FileTools::FileManager::setColumnName(ncn, pcn);

				process_grouping(nc, pc);

				log_msg->setLog(module, "Grouping", JSONTools::LOGINFO, "Grouping process finished");
				SnowProcessHelperUI.Send(log_msg->toJSON().c_str());
			}

			break;
		}
		default:
			break;
		}
		text = "";
	}
	SnowProcessHelperUI.close();
	system("PAUSE");
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Conseils pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
