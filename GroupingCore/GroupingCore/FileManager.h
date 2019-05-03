#pragma once

#include "framework.h"

class FileManager
{
public:
	FileManager() {};
	~FileManager() {};

	virtual std::list<grouping::Node> read_file(std::string path, int node_column, int property_column, bool case_sensitive = false) = 0;

	static std::string dir;
	static std::string output_fileName;
};