#pragma once

#ifdef GROUPINGCORE_EXPORTS
#define GROUPINGCORE_API __declspec(dllexport)
#else
#define GROUPINGCORE_API __declspec(dllimport)
#endif

#include "FileManager.h"
#include <fstream>
#include <iostream>
#include <conio.h>
#include <libXL/libxl.h>


namespace FileTools
{
	class GROUPINGCORE_API XLSXTools : public FileManager
	{
	public:
		XLSXTools() {};
		~XLSXTools() {};

		std::list<grouping::Node> read_file(std::string path, int node_column, int property_column, bool case_sensitive = false);
	};
}