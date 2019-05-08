#include "XLSXTools.h"
#include "StringTools.h"

using namespace std;
using namespace libxl;

namespace FileTools
{
	list<grouping::Node> XLSXTools::read_file(string path, int node_column, int property_column, bool case_sensitive)
	{
		Book* book = xlCreateXMLBook();
		list<grouping::Node> nodes;
		string input_fileName = "", ext = "";
		char _dir[2048] = { 0 }; // stockera le fullpath renvoyée par la fonction _fullpath()

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

		book->load(StringTools::string2wchar(path));

		if (!book)
		{
			cout << "Impossible d'ouvrir le fichier " << path << endl;
			return nodes;
		}

		Sheet* sheet = book->getSheet(0);

		if (sheet)
			for (int row = sheet->firstRow()+1; row < sheet->lastRow(); ++row)
			{
				string nodeStr = (sheet->cellType(row, node_column) == CELLTYPE_STRING) ? StringTools::wchar2string(sheet->readStr(row, node_column)) : "";
				string propertyStr = (sheet->cellType(row, property_column) == CELLTYPE_STRING) ? StringTools::wchar2string(sheet->readStr(row, property_column)) : "";

				if (nodeStr.length() > 0)
				{
					grouping::Node n = grouping::Node(nodeStr);
					nodes.push_back(n);
				}

				if (propertyStr.length() > 0)
					if (case_sensitive)
						nodes.back().properties.push_back(propertyStr);
					else
						nodes.back().properties.push_back(StringTools::toLower(propertyStr));
			}

		book->release();
		
		return nodes;
	}

}