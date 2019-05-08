#include "CSVTools.h"
#include "StringTools.h"

using namespace std;

namespace FileTools
{
	std::string FileManager::dir = "";
	std::string FileManager::output_fileName = "";
	const char* FileManager::EXTENSION[] = { "csv", "xlsx" };

	list<grouping::Node> CSVTools::read_file(string path, int node_column, int property_column, bool case_sensitive)
	{
		list<grouping::Node> nodes;
		string input_fileName = "", ext = "";
		char _dir[2048] = { 0 }; // stockera le fullpath renvoyée par la fonction _fullpath()
		ifstream fichier(path.c_str(), ios::in);  // on ouvre le fichier en lecture

		if (!fichier)
		{
			cout << "Impossible d'ouvrir le fichier " << path << endl;
			return nodes;
		}

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
		FileManager::output_fileName = (string::npos == pos) ? "" : input_fileName.substr(0, pos) + "_output";
		FileManager::output_fileName += input_fileName.substr(pos, input_fileName.length() - 1);

		
		getline(fichier, ligne);  // on met dans "contenu" la ligne
		//TODO : gestion de l'entête

		while (getline(fichier, ligne))
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

		fichier.close();  // on ferme le fichier
		
		return nodes;
	}
}