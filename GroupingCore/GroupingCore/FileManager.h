#pragma once

#include "framework.h"
#include "ProcessException.h"

namespace FileTools
{
	enum {CSV = 0, XLSX = 1};

	class GROUPINGCORE_API FileManager
	{
	public:
		FileManager() {};
		~FileManager() {};

		virtual std::list<grouping::Node> read_file(std::string path, int node_column, int property_column, bool case_sensitive = false) = 0;
		virtual void write_file(std::list<grouping::Node>* nodes, grouping::Group* g) = 0;

		static std::string dir;
		static std::string m_output_fileName;
		static std::string m_node_columnStr;
		static std::string m_property_columnStr;
		static const char* EXTENSION[];

		static void setColumnName(std::string node_column, std::string property_column) {
			m_node_columnStr = node_column;
			m_property_columnStr = property_column;
		};
		static std::string getFilename(std::string fullpath) {
			size_t pos = fullpath.find_last_of("\\/");
			return (std::string::npos == pos) ? "" : fullpath.substr(pos + 1, fullpath.length() - 1);
		}
	};
}