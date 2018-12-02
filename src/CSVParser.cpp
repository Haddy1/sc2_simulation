#include "../include/CSVParser.h"

#include <sstream>
#include <iostream> //debug

using std::istringstream;
using std::cout;
using std::endl;

CSVParser::CSVParser(const char *filepath) : filepath(filepath) {
	
}

void CSVParser::read() {
	ifstream is(filepath);

	while (is.good()) {
		string s;
		getline(is, s);
		lines.push_back(s);
	}
	
	is.close();
	

	//flags
	int currentRace = -1;
	int currentType = -1; //0:unit 1:building //todo enum

	for (auto it = lines.begin(); it != lines.end(); ++it) {
		string line = *it;
		//istringstream iss(line);
		//string token;
		//getline(iss, token, ',');
		if (line.substr(0, 1).find("#") != string::npos) {
			//line starts with #
			if (line.find("Protoss") != string::npos) {
				//starting to read protoss data
				currentRace = 0;
				currentType = 0;
				cout << "Protoss Start: " << line << endl;
			} else if (line.find("Zerg") != string::npos) {
				currentRace = 1;
				currentType = 0;
				cout << "Zerg Start: " << line << endl;
			} else if (line.find("Terran") != string::npos) {
				currentRace = 2;
				currentType = 0;
				cout << "Terran Start: " << line << endl;
			} else if (line.find("#buildings") != string::npos) {
				//starting to read buildings
				currentType = 1;
				cout << "Buildings Start: " << line << endl;
			} else {
				cout << "Misc Line: " << line << endl;
				continue;
			}
		} else {
			//line does not start with #
			if (line.size() == 0) {
				//reached end
				cout << "End: " << line << endl;
			}
			else {
				//normal line
				cout << "Normal: " << line << endl;
				//parse
				istringstream iss(line);
				string token;

				getline(iss, token, ','); //name
				//do something with token
				getline(iss, token, ','); //minerals
				//...
				
			}
		}

	}

}

CSVParser::~CSVParser() {
	
}
