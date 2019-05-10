// pch.cpp : fichier source correspondant à l'en-tête précompilé

#include "grouping.h"
#include <iostream>



using namespace std;
 
namespace grouping
{
	GroupingTools::GroupingTools() {}

	void GroupingTools::run() {
		SetConsoleOutputCP(65001);
	}

	void GroupingTools::to_prune(map<string, Properties > source, map<string, Properties > * pruned_tree, list<pair<string, int> > * pscore)
	{
		map<string, int> d_score;
		list<int> score_values;

		//iterators
		map<string, Properties>::iterator source_it;
		map<string, int>::iterator score_it;
		list<string>::iterator prop_it;
		list<int>::iterator score_val_it;

		cout << "Copie de l'arbre source" << endl;
		*pruned_tree = source;

		cout << "Comptage de la fréquence des propriétés" << endl;
		for (source_it = source.begin(); source_it != source.end(); source_it++)
			for (prop_it = source_it->second.begin(); prop_it != source_it->second.end(); prop_it++)
				if (d_score.count(*prop_it))
					d_score[*prop_it]++;
				else
					d_score[*prop_it] = 1;

		/*cout << "DEBUG -> Scores bruts : " << endl;
		for (score_it = d_score.begin(); score_it != d_score.end(); score_it++)
			cout << score_it->first << " : " << score_it->second << endl;*/

		cout << "Suppression des propriétés uniques" << endl;
		for (score_it = d_score.begin(); score_it != d_score.end(); score_it++)
			if (score_it->second > 1)
				score_values.push_back(score_it->second);
			else 
				for (source_it = pruned_tree->begin(); source_it != pruned_tree->end(); source_it++)
				{
					Properties node_properties = pruned_tree->at(source_it->first);
					string target = score_it->first;

					if (find(node_properties.begin(), node_properties.end(), target) != node_properties.end())
					{
						pruned_tree->at(source_it->first).remove(target); 
						source_it = next(pruned_tree->end(), -1);
					}
				}

		score_values.sort(std::greater<int>());
		score_values.unique();

		cout << "Enregistrement des propriétés triées selon leur score (ordre décroissant)" << endl;
		for (score_val_it = score_values.begin(); score_val_it != score_values.end(); score_val_it++)
			for (score_it = d_score.begin(); score_it != d_score.end(); score_it++)
				if (score_it->second == *score_val_it)
					pscore->push_back(pair<string, int>(score_it->first, score_it->second));
	}

	bool GroupingTools::pair_verification(map<string, Properties > * pruned_tree, string p1, string p2)
	{
		map<string, Properties>::iterator tree_it;

		for (tree_it = pruned_tree->begin(); tree_it != pruned_tree->end(); tree_it++)
		{
			Properties node_properties = pruned_tree->at(tree_it->first);
			bool contains_p1 = (find(node_properties.begin(), node_properties.end(), p1) != node_properties.end());
			bool contains_p2 = (find(node_properties.begin(), node_properties.end(), p2) != node_properties.end());

			if ((contains_p1 && !contains_p2) || (contains_p2 && !contains_p1))
				return false;
		}

		return true;
	}

	Group GroupingTools::grouping(list<Node> source_nodes)
	{
		map<string, Properties > pruned_tree;
		list<pair<string, int> > pscore; //contains the properties score
		map<int, Properties > O; //occurence array
		Properties p; //properties which have the same occurence
		Group result;
		bool grouped;

		//data preparation
		map<string, Properties > source = nodes2map(&source_nodes);
		cout << "Elaguage de l'arbre d'entrée" << endl;
		to_prune(source, &pruned_tree, &pscore);


		/*cout << "DEBUG -> L'arbre élagué est : " << endl;
		map<string, Properties >::iterator d_it;
		for (d_it = pruned_tree.begin(); d_it != pruned_tree.end(); d_it++)
		{
			cout << d_it->first << " :" << endl;
			Properties::iterator d2_it;
			for (d2_it = d_it->second.begin(); d2_it != d_it->second.end(); d2_it++)
				cout << "    " << *d2_it << endl;
		}
		cout << endl << endl;*/


		cout << "Remplissage du tableau d'occurence" << endl;
		list<pair<string, int> >::iterator pscore_it;
		for (pscore_it = pscore.begin(); pscore_it != pscore.end(); pscore_it++)
			O[pscore_it->second].push_back(pscore_it->first);


		/*cout << "DEBUG -> Le tableau d'occurence est : " << endl;
		map<int, Properties >::iterator d3_it;
		for (d3_it = O.begin(); d3_it != O.end(); d3_it++)
		{
			cout << d3_it->first << " :" << endl;
			Properties::iterator d4_it;
			for (d4_it = d3_it->second.begin(); d4_it != d3_it->second.end(); d4_it++)
				cout << "    " << *d4_it << endl;
		}
		cout << endl << endl;*/


		cout << "Recherche des propriétés pouvant être regroupées" << endl;
		map<int, Properties >::iterator O_it;
		for (O_it = O.begin(); O_it != O.end(); O_it++)
		{
			p = O_it->second;
			Properties::iterator i;
			Properties::iterator j;
			
			for (i = p.begin(); i != p.end()--; i++)
			{
				grouped = false;
				string p1 = *i;

				for (j = next(i, 1); j != p.end(); j++)
				{
					string p2 = *j;

					if (pair_verification(&pruned_tree, p1, p2))
					{
						cout << p1 << " et " << p2 << " peuvent être groupées" << endl;

						grouped = true;
						Group::iterator group_it;

						if (!is_p_grouped(&result, p1))
						{
							string group_name = p1 + "_group";
							cout << "Création du groupe " << group_name << " contenant " << p1 << " et " << p2 << endl;

							result[group_name].push_back(p1);
							result[group_name].push_back(p2);
						}
						else
						{
							string p1_group_name = get_group_name(&result, p1);
							string p2_group_name = get_group_name(&result, p2);

							if (p1_group_name.compare(p2_group_name) != 0)
							{
								cout << "Le groupe " << p1_group_name << " s'est vu rajouté la propriété " << p2 << endl;
								result[p1_group_name].push_back(p2);
							}
						}
					}
					else
						ask_for_grouping(&result, p1);
				}
				if (!grouped)
					ask_for_grouping(&result, p1);
			}
		}

		return result;
	}

	void GroupingTools::ask_for_grouping(Group * g, string p)
	{
		if (!is_p_grouped(g, p))
		{
			string group_name = p + "_group";
			cout << "Création du groupe " << group_name << " contenant " << p << endl;
			Properties plist;
			plist.push_back(p);
			g->insert(pair<string, Properties>(group_name,plist));
		}
	}

	bool GroupingTools::is_p_grouped(Group* g, string p)
	{
		Group::iterator group_it;
		for (group_it = g->begin(); group_it != g->end(); group_it++)
		{
			Properties group_properties = group_it->second;
			if (find(group_properties.begin(), group_properties.end(), p) != group_properties.end())
				return true;
		}
		return false;
	}

	bool GroupingTools::is_p_in_node(Properties* psource, string p)
	{
		Properties::iterator it;

		for (it = psource->begin(); it != psource->end(); it++)
		{
			if (it->compare(p) == 0)
				return true;
		}
		return false;
	}
	string GroupingTools::get_group_name(Group * g, string prop)
	{
		Group::iterator group_it;
		for (group_it = g->begin(); group_it != g->end(); group_it++)
		{
			Properties group_properties = g->at(group_it->first);
			if (find(group_properties.begin(), group_properties.end(), prop) != group_properties.end())
				return group_it->first;
		}
		return "";
	}

	void GroupingTools::show_groups(Group* g)
	{
		Group::iterator group_it;
		int gr_nb = 0;
		int p_nb = 0;
		for (group_it = g->begin(); group_it != g->end(); group_it++)
		{
			cout << "<" << group_it->first << ">" << endl;
			gr_nb++;

			Properties group_properties = g->at(group_it->first);
			Properties::iterator p_it;
			for (p_it = group_properties.begin(); p_it != group_properties.end(); p_it++)
			{
				p_nb++;
				cout << "  -> " << *p_it << endl;
			}
		}
		cout << gr_nb << " ont été créés, regroupant au total " << p_nb << " propriétés" << endl;
	}

	map<string, Properties > GroupingTools::nodes2map(list<Node>* source)
	{
		list<Node>::iterator node_it;
		map<string, Properties > result;

		for (node_it = source->begin(); node_it != source->end(); node_it++)
			for (Properties::iterator p_it = node_it->properties.begin(); p_it != node_it->properties.end(); p_it++)
				result[node_it->name].push_back(*p_it);

		return result;
	}

	list<Node> GroupingTools::buildTreeWithGroups(list<Node> * lnodes, Group * group)
	{
		Group::iterator group_it;

		list<Node> result;
		list<Node>::iterator node_it;

		for (node_it = lnodes->begin(); node_it != lnodes->end(); node_it++)
		{
			Node n(node_it->name);
			Properties::iterator p_it;

			for (p_it = node_it->properties.begin(); p_it != node_it->properties.end(); p_it++)
			{
				string groupName = get_group_name(group, *p_it);

				if (groupName.length() <= 0)
					n.properties.push_back(*p_it);
				else if (!is_p_in_node(&n.properties, groupName))
					n.properties.push_back(groupName);
			}

			result.push_back(n);
		}
		return result;
	}

	list<Node> GroupingTools::map2nodes(map<string, Properties >* source)
	{
		list<Node> result;
		map<string, Properties >::iterator it;

		for (it = source->begin(); it != source->end(); it++)
		{
			Properties::iterator p_it;
			Node n(it->first);
			for (p_it = it->second.begin(); p_it != it->second.end(); p_it++)
				n.properties.push_back(*p_it);
		}
		return result;
	}
}