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
	
	int currentRace;
	int currentType; //0:unit 1:building //todo enum
	
	void parseLine(string);
	void parseUnitLine(string);
	void parseBuildingLine(string);
public:
	CSVParser(const char *filepath);
	void read();
	~CSVParser();
};
