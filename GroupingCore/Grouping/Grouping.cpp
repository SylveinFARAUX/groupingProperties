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

int main()
{
	FileTools::FileManager* file_manager;

	 /* DATA DEMO */
	string file = "C:\\Users\\sylveinfaraux\\Documents\\personnel\\workspace\\groupingProperties\\GroupingCore\\entree.xlsx";
	int node_column = 0;
	int property_column = 1;

	string filename = FileTools::FileManager::getFilename(file);
	vector <string> filename_array;
	StringTools::split(filename, '.', filename_array);
	string extension = filename_array[1];

	/*if (extension.compare(FileTools::FileManager::EXTENSION[FileTools::CSV]) == 0)
		file_manager = new FileTools::CSVTools();
	else
		file_manager = new FileTools::XLSXTools();

	list<grouping::Node> lno = file_manager->read_file(file, node_column, property_column);

	cout << "L'arbre est : " << endl;

	list<grouping::Node>::iterator it;
	for (it = lno.begin(); it != lno.end(); it++)
	{
		cout << "<" << it->name << ">" << endl;

		grouping::Properties::iterator p_it;

		for (p_it = it->properties.begin(); p_it != it->properties.end(); p_it++)
			cout << " -> " << *p_it << endl;
		cout << endl;
	}

	//show_groups(&grouping(lno));//*/
	string jsonmsg = "{\"hello\": \"world\", \"t\" : true, \"f\" : false, \"n\" : null, \"i\" : 123, \"pi\" : 3.1416, \"a\" : [1, 2, 3, 4]}";
	
	bool testBool;
	
	try{
		testBool = JSONTools::getBoolValueFromJSON(jsonmsg, "i");
	}
	catch (const ProcessException & e){
		e.what();
	}

	JSONTools::JSONConnectionCommand jsonConnect("WeTheBest");
	cout << jsonConnect.toJSON() << endl;
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
