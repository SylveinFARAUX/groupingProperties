#pragma once

#include "framework.h"
#include <fstream>
#include <iostream>

namespace FileTools
{
	class CSVTools
	{
	public:
		static std::list<grouping::Node> read_file(std::string path, int node_column, int property_column, bool case_sensitive = false);
		
	private:
		static std::string dir;
		static std::string output_fileName;
		CSVTools() {}
		~CSVTools() {}
	};
}
