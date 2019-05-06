#pragma once

#include "FileManager.h"
#include <fstream>
#include <iostream>

//#pragma comment(lib, "xlnt.lib")

namespace FileTools
{
	class XLSXTools : public FileManager
	{
	public:
		XLSXTools() {};
		~XLSXTools() {};

		std::list<grouping::Node> read_file(std::string path, int node_column, int property_column, bool case_sensitive = false);
	};
}