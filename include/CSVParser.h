#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Race.h"

using std::ifstream;
using std::string;
using std::vector;

class CSVParser {
	const char *filepath;
	//ifstream is;
	vector<string> lines;
	
	Race currentRace;
	int currentType; //0:unit 1:building
	
	void parseLine(string);
	void parseEntityLine(string);
	//void parseUnitLine(string);
	//void parseBuildingLine(string);
public:
	CSVParser(const char *filepath);
	void parse();
	~CSVParser();
};
