#pragma once

#include "framework.h"

namespace FileTools
{
	enum {CSV = 0, XLSX = 1};

	class GROUPINGCORE_API FileManager
	{
	public:
		FileManager() {};
		~FileManager() {};

		virtual std::list<grouping::Node> read_file(std::string path, int node_column, int property_column, bool case_sensitive = false) = 0;

		static std::string dir;
		static std::string output_fileName;
		static const char* EXTENSION[];

		static std::string getFilename(std::string fullpath) {
			size_t pos = fullpath.find_last_of("\\/");
			return (std::string::npos == pos) ? "" : fullpath.substr(pos + 1, fullpath.length() - 1);
		}
	};
}