#include "CSVTools.h"
#include "StringTools.h"

using namespace std;

namespace FileTools
{
	string FileManager::dir = "";
	string FileManager::m_output_fileName = "";
	string FileManager::m_node_columnStr = "";
	string FileManager::m_property_columnStr = "";
	const char* FileManager::EXTENSION[] = { "csv", "xlsx" };

	list<grouping::Node> CSVTools::read_file(string path, int node_column, int property_column, bool case_sensitive)
	{
		list<grouping::Node> nodes;
		string input_fileName = "", ext = "";
		char _dir[2048] = { 0 }; // stockera le fullpath renvoyée par la fonction _fullpath()
		ifstream file(path.c_str(), ios::in);  // on ouvre le fichier en lecture

		if (!file)
			throw ProcessException(ProcessException::FATAL, ProcessException::FILE_OPEN, "Impossible d'ouvrir le fichier " + path);

		string ligne;

		//récupère le chemin complet de l'exécutable
		FileManager::dir = _fullpath(_dir, __FILE__, 2048);

		//ne garder que le chemin du répertoire courant
		size_t pos = FileManager::dir.find_last_of("\\/");
		FileManager::dir = (string::npos == pos) ? "" : FileManager::dir.substr(0, pos);
		FileManager::dir += "\\";

		pos = path.find_last_of("\\/");
		input_fileName = (string::npos == pos) ? "" : path.substr(pos + 1, path.length() - 1);

		pos = input_fileName.find_last_of(".");
		FileManager::m_output_fileName = (string::npos == pos) ? "" : input_fileName.substr(0, pos) + "_output." + FileManager::EXTENSION[CSV];
		
		getline(file, ligne);

		while (getline(file, ligne))
		{
			vector<string> l;
			StringTools::split(ligne, ';', l);

			if (l[node_column].length() > 0)
			{
				grouping::Node n = grouping::Node(l[node_column]);
				nodes.push_back(n);
			}

			if (l[property_column].length() > 0)
				if (case_sensitive)
					nodes.back().properties.push_back(l[property_column]);
				else
					nodes.back().properties.push_back(StringTools::toLower(l[property_column]));
		}

		file.close();
		
		return nodes;
	}

	void CSVTools::write_file(list<grouping::Node>* nodes, grouping::Group* group)
	{
		ofstream file(FileManager::m_output_fileName, ios::out | ios::trunc);
		list<grouping::Node>::iterator it;
		grouping::Group::iterator g_it;

		if (!file)
			throw ProcessException(ProcessException::FATAL, ProcessException::FILE_OPEN, "Impossible d'ouvrir le fichier "+ FileManager::m_output_fileName);

			
		file << FileManager::m_node_columnStr << ";" << FileManager::m_property_columnStr << endl;

		//Write tree
		for (it = nodes->begin(); it != nodes->end(); it++)
		{
			file << it->name << ";";
			grouping::Properties::iterator p_it;

			for (p_it = it->properties.begin(); p_it != it->properties.end(); p_it++)
				if (p_it == it->properties.begin())
					file << *p_it << endl;
				else
					file << ";" << *p_it << endl;
			file << ";" << endl;
		}
		
		file << ";" << endl << "Groups :;" << endl;

		//Write groups
		for (g_it = group->begin(); g_it != group->end(); g_it++)
		{
			file << g_it->first << ";";
			grouping::Properties::iterator p_it;
			
			for (p_it = g_it->second.begin(); p_it != g_it->second.end(); p_it++)
				if (p_it == g_it->second.begin())
					file << *p_it << endl;
				else
					file << ";" << *p_it << endl;
		}

		file.close();
	}
}