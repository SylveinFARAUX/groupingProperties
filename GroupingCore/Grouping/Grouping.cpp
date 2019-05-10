// Grouping.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <string>
#include <vector>

#include <GroupingCore/JSONTools.h>

#include <GroupingCore/grouping.h>

#include <GroupingCore/CSVTools.h>
#include <GroupingCore/XLSXTools.h>
#include <GroupingCore/StringTools.h>

using namespace std;

string file;
FileTools::FileManager* file_manager;

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

	list<grouping::Node> lno = file_manager->read_file(file, node_column, property_column);

	/*cout << "L'arbre est : " << endl;

	list<grouping::Node>::iterator it;
	for (it = lno.begin(); it != lno.end(); it++)
	{
		cout << "<" << it->name << ">" << endl;

		grouping::Properties::iterator p_it;

		for (p_it = it->properties.begin(); p_it != it->properties.end(); p_it++)
			cout << " -> " << *p_it << endl;
		cout << endl;
	}*/

	//process grouping

	grouping::Group groups = grouping::GroupingTools::grouping(lno);
	grouping::GroupingTools::show_groups(&groups);//*/
	list<grouping::Node> result = grouping::GroupingTools::buildTreeWithGroups(&lno, &groups);

	//save the results
	file_manager->write_file(&result, &groups);
}

int main()
{
	

	 /* CASE Process -> case grouping */
	//set_file("C:\\Users\\sylveinfaraux\\Documents\\personnel\\workspace\\groupingProperties\\GroupingCore\\entree.xlsx");
	set_file("C:\\Users\\sylveinfaraux\\Documents\\ServiceNow\\Training\\Catalog Item Variables.xlsx");
	FileTools::FileManager::setColumnName("Entity", "Property");
	process_grouping(1, 3);

	/*string jsonmsg = "{\"hello\": \"world\", \"t\" : true, \"f\" : false, \"n\" : null, \"i\" : 123, \"pi\" : 3.1416, \"a\" : [1, 2, 3, 4]}";
	
	bool testBool;
	
	try{
		testBool = JSONTools::getBoolValueFromJSON(jsonmsg, "i");
	}
	catch (const ProcessException & e){
		e.what();
	}

	JSONTools::JSONConnectionCommand jsonConnect("WeTheBest");
	cout << jsonConnect.toJSON() << endl;*/
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
