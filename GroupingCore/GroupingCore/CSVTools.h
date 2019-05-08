#pragma once

#include "FileManager.h"

#include <fstream>
#include <iostream>

namespace FileTools
{
	class GROUPINGCORE_API CSVTools : public FileManager
	{
	public:
		CSVTools() {}
		~CSVTools() {}

		std::list<grouping::Node> read_file(std::string path, int node_column, int property_column, bool case_sensitive = false);
	};
}
