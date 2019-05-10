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
		FileManager::m_output_fileName = (string::npos == pos) ? "" : input_fileName.substr(0, pos) + "_output." + FileManager::EXTENSION[XLSX];

		book->load(StringTools::string2wchar(path));

		if (!book)
			throw ProcessException(ProcessException::FATAL, ProcessException::FILE_OPEN, "Impossible d'ouvrir le fichier " + FileManager::m_output_fileName);


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

	void XLSXTools::write_file(list<grouping::Node>* nodes, grouping::Group* group)
	{
		Book* book = xlCreateXMLBook();
		list<grouping::Node>::iterator it;
		grouping::Group::iterator g_it;

		if (!book)
			throw ProcessException(ProcessException::FATAL, ProcessException::FILE_OPEN, "Impossible d'ouvrir le fichier " + FileManager::m_output_fileName);


		Sheet * sheet = book->addSheet(L"Results");
		sheet->setDisplayGridlines(true);

		sheet->writeStr(1, 0, StringTools::string2wchar(FileManager::m_node_columnStr));
		sheet->writeStr(1, 1, StringTools::string2wchar(FileManager::m_property_columnStr));

		int i = 2;

		//Write tree
		for (it = nodes->begin(); it != nodes->end(); it++)
		{
			sheet->writeStr(i, 0, StringTools::string2wchar(it->name));

			grouping::Properties::iterator p_it;

			for (p_it = it->properties.begin(); p_it != it->properties.end(); p_it++)
				if (p_it == it->properties.begin())
					sheet->writeStr(i, 1, StringTools::string2wchar(*p_it));
				else
				{
					i++;
					sheet->writeStr(i, 1, StringTools::string2wchar(*p_it));
				}
			i = i + 2;
		}

		i++;
		sheet->writeStr(i, 0, L"Groups : ");
		i++;

		//Write groups
		for (g_it = group->begin(); g_it != group->end(); g_it++)
		{
			sheet->writeStr(i, 0, StringTools::string2wchar(g_it->first));
			grouping::Properties::iterator p_it;

			for (p_it = g_it->second.begin(); p_it != g_it->second.end(); p_it++)
				if (p_it == g_it->second.begin())
					sheet->writeStr(i, 1, StringTools::string2wchar(*p_it));
				else
				{
					i++;
					sheet->writeStr(i, 1, StringTools::string2wchar(*p_it));
				}
			i++;
		}

		book->save(StringTools::string2wchar(FileTools::FileManager::m_output_fileName));
		book->release();
	}
}