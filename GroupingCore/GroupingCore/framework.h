#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclure les en-têtes Windows rarement utilisés
// Fichiers d'en-tête Windows
#include <windows.h>
#include <string>
#include <list>
#include <map>
#include <algorithm>
#include <iterator>

namespace grouping
{
	typedef std::list<std::string> Properties;
	typedef std::map<std::string, Properties > Group;

	struct Node {
		std::string name;
		Properties properties;
		Node(std::string n) {
			name = n;
		}
	};
}
