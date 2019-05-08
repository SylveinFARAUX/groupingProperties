// pch.h : Il s'agit d'un fichier d'en-tête précompilé.
// Les fichiers listés ci-dessous sont compilés une seule fois, ce qui améliore les performances de génération des futures builds.
// Cela affecte également les performances d'IntelliSense, notamment la complétion du code et de nombreuses fonctionnalités de navigation du code.
// Toutefois, les fichiers listés ici sont TOUS recompilés si l'un d'entre eux est mis à jour entre les builds.
// N'ajoutez pas de fichiers fréquemment mis à jour ici, car cela annule les gains de performance.

#pragma once

#include "framework.h"

namespace grouping
{
	class GROUPINGCORE_API GroupingTools
	{
	public:
		GroupingTools();
		static void run();
		static Group grouping(std::list<Node> source_nodes);

	private:
		static void to_prune(std::map<std::string, Properties > source, std::map<std::string, Properties >* pruned_tree, std::list<std::pair<std::string, int> >* pscore);
		static bool pair_verification(std::map<std::string, Properties >* pruned_tree, std::string p1, std::string p2);
		static std::map<std::string, Properties > nodes2map(std::list<Node> source);

		static bool is_p_grouped(Group* g, std::string p);
		static void ask_for_grouping(Group* g, std::string p);
		static std::string get_group_name(Group* g, std::string prop);
		static void show_groups(Group* g);
	};
}