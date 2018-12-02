#include "../include/CSVParser.h"
#include "../include/UnitData.h"

#include <sstream>
#include <iostream> //debug
#include <cctype>
#include <algorithm>

using std::istringstream;
using std::cout;
using std::endl;
using std::stoi;

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

				if (currentType == 0) {
				
					UnitData unitData;

					getline(iss, token, ','); //name
					unitData.name = token;
					getline(iss, token, ',');
					unitData.minerals = stoi(token);
					getline(iss, token, ',');
					unitData.vespene = stoi(token);
					getline(iss, token, ',');
					unitData.buildTime = stoi(token);
					getline(iss, token, ',');
					unitData.supplyCost = stoi(token);
					getline(iss, token, ',');
					unitData.supplyProvided = stoi(token);
					getline(iss, token, ',');
					unitData.startEnergy = stoi(token);
					getline(iss, token, ',');
					unitData.maxEnergy = stoi(token);
					getline(iss, token, ',');
					unitData.race = currentRace;
					
					getline(iss, token, ',');
					vector<string> prods;
					istringstream iss2(token);
					string subToken;
					while(iss2.good()) { //ugly control flow
						getline(iss2, subToken, '/');
						if (subToken.size() > 0) {
							subToken.erase(remove_if(subToken.begin(), subToken.end(), isspace), subToken.end());//remove spaces
							prods.push_back(subToken);
						}
					}
					unitData.producedBy = prods;

					getline(iss, token, ',');
					vector<string> deps;
					istringstream iss3(token);
					string subToken2;
					while(iss3.good()) { //ugly control flow
						getline(iss3, subToken2, '/');
						if (subToken2.size() > 0) {
							subToken2.erase(remove_if(subToken2.begin(), subToken2.end(), isspace), subToken2.end());
							deps.push_back(subToken2);
						}
					}
					unitData.dependencies = deps;

					unitDataMap.emplace(unitData.name, unitData);

				}
			}
		}

	}

}

CSVParser::~CSVParser() {
	
}
