#include "XLSXTools.h"

#include <xlnt/xlnt.hpp>
using namespace std;

namespace FileTools
{
	list<grouping::Node> XLSXTools::read_file(string path, int node_column, int property_column, bool case_sensitive)
	{
		xlnt::workbook wb;
		/*wb.load(path);

		xlnt::worksheet workbook_sheet = wb.active_sheet();

		/*for (auto row : workbook_sheet.rows(false))
		{
			//cout << row[0].to_string() << endl;
		}*/
		return list<grouping::Node>();
	}

}