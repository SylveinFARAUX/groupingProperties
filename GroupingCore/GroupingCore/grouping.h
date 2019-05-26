#pragma once

#include "framework.h"

/* Communication mangement */
#include "Socket.h"
#include "JSONTools.h"



	class GROUPINGCORE_API GroupingTools
	{
	public:
		GroupingTools();
		static void run();
		static grouping::Group grouping(std::list<grouping::Node> source_nodes);
		static std::list<grouping::Node> buildTreeWithGroups(std::list<grouping::Node>* lnodes, grouping::Group* group);
		static void show_groups(grouping::Group* g);
		static std::map<std::string, grouping::Properties > nodes2map(std::list<grouping::Node>* source);
		static std::list<grouping::Node> map2nodes(std::map<std::string, grouping::Properties >* source);
		static void connectToUI();
		static void closeConnection();

	private:
		static void to_prune(std::map<std::string, grouping::Properties > source, std::map<std::string, grouping::Properties >* pruned_tree, std::list<std::pair<std::string, int> >* pscore);
		static bool pair_verification(std::map<std::string, grouping::Properties >* pruned_tree, std::string p1, std::string p2);

		static bool is_p_in_node(grouping::Properties* psource, std::string p);
		static bool is_p_grouped(grouping::Group* g, std::string p);
		static void ask_for_grouping(grouping::Group* g, std::string p);
		static std::string get_group_name(grouping::Group* g, std::string prop);
		static void send_log(JSONTools::JSONCommand* jsonMessage);
		

		static Socket* coreSock;
	};

