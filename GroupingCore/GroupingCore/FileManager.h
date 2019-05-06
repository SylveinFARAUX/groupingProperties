#pragma once

#include "framework.h"

namespace FileTools
{
	enum {CSV = 0, XLSX = 1};

	class FileManager
	{
	public:
		FileManager() {};
		~FileManager() {};

		virtual std::list<grouping::Node> read_file(std::string path, int node_column, int property_column, bool case_sensitive = false) = 0;

		static std::string dir;
		static std::string output_fileName;
		static const char* EXTENSION[];
	};
}