#pragma once

#include <fstream>
#include <string>
#include <vector>

using std::ifstream;
using std::string;
using std::vector;

class CSVParser {
	const char *filepath;
	//ifstream is;
	vector<string> lines;
public:
	CSVParser(const char *filepath);
	void read();
	~CSVParser();
};
