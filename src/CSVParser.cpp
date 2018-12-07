#include "../include/CSVParser.h"
//#include "../include/UnitData.h"
//#include "../include/BuildingData.h"
#include "../include/EntityData.h"

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

void CSVParser::parseEntityLine(string line) {
	istringstream iss(line);
	string token;
	
	EntityData entityData;
	
	if (currentType == 0) {
		entityData.isBuilding = false;
	} else {
		entityData.isBuilding = true;
	}

	getline(iss, token, ','); //name
	entityData.name = token;
	getline(iss, token, ',');
	entityData.minerals = stoi(token);
	getline(iss, token, ',');
	entityData.vespene = stoi(token);
	getline(iss, token, ',');
	entityData.buildTime = stoi(token);
	getline(iss, token, ',');
	entityData.supplyCost = stod(token);
	getline(iss, token, ',');
	entityData.supplyProvided = stoi(token);
	getline(iss, token, ',');
	entityData.startEnergy = stoi(token);
	getline(iss, token, ',');
	entityData.maxEnergy = stoi(token);
	getline(iss, token, ',');
	entityData.race = currentRace;
					
	getline(iss, token, ',');
	vector<string> prods;
	istringstream iss2(token);
	string subToken;
	while(iss2.good()) {
		getline(iss2, subToken, '/');
		if (subToken.size() > 0) {
			subToken.erase(remove_if(subToken.begin(), subToken.end(), isspace), subToken.end());//remove spaces
			prods.push_back(subToken);
		}
	}
	entityData.producedBy = prods;

	getline(iss, token, ',');
	vector<string> deps;
	istringstream iss3(token);
	string subToken2;
	while(iss3.good()) {
		getline(iss3, subToken2, '/');
		if (subToken2.size() > 0) {
			subToken2.erase(remove_if(subToken2.begin(), subToken2.end(), isspace), subToken2.end());
			deps.push_back(subToken2);
		}
	}
	entityData.dependencies = deps;

	entityDataMap.emplace(entityData.name, entityData);
}

/*
void CSVParser::parseUnitLine(string line) {
	istringstream iss(line);
	string token;
	
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
	while(iss2.good()) {
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
	while(iss3.good()) {
		getline(iss3, subToken2, '/');
		if (subToken2.size() > 0) {
			subToken2.erase(remove_if(subToken2.begin(), subToken2.end(), isspace), subToken2.end());
			deps.push_back(subToken2);
		}
	}
	unitData.dependencies = deps;

	unitDataMap.emplace(unitData.name, unitData);
}

void CSVParser::parseBuildingLine(string line) {
	istringstream iss(line);
	string token;
	
	BuildingData buildingData;

	getline(iss, token, ','); //name
	buildingData.name = token;
	getline(iss, token, ',');
	buildingData.minerals = stoi(token);
	getline(iss, token, ',');
	buildingData.vespene = stoi(token);
	getline(iss, token, ',');
	buildingData.buildTime = stoi(token);
	getline(iss, token, ',');
	buildingData.supplyCost = stoi(token);
	getline(iss, token, ',');
	buildingData.supplyProvided = stoi(token);
	getline(iss, token, ',');
	buildingData.startEnergy = stoi(token);
	getline(iss, token, ',');
	buildingData.maxEnergy = stoi(token);
	getline(iss, token, ',');
	buildingData.race = currentRace;
					
	getline(iss, token, ',');
	vector<string> prods;
	istringstream iss2(token);
	string subToken;
	while(iss2.good()) {
		getline(iss2, subToken, '/');
		if (subToken.size() > 0) {
			subToken.erase(remove_if(subToken.begin(), subToken.end(), isspace), subToken.end());//remove spaces
			prods.push_back(subToken);
		}
	}
	buildingData.producedBy = prods;

	getline(iss, token, ',');
	vector<string> deps;
	istringstream iss3(token);
	string subToken2;
	while(iss3.good()) {
		getline(iss3, subToken2, '/');
		if (subToken2.size() > 0) {
			subToken2.erase(remove_if(subToken2.begin(), subToken2.end(), isspace), subToken2.end());
			deps.push_back(subToken2);
		}
	}
	buildingData.dependencies = deps;

	buildingDataMap.emplace(buildingData.name, buildingData);
}
*/

void CSVParser::parseLine(string line) {
	if (line.substr(0, 1).find("#") != string::npos) {
		//line starts with #
		if (line.find("Protoss") != string::npos) {
			//starting to read protoss data
			currentRace = Race::PROTOSS;
			currentType = 0;
		} else if (line.find("Zerg") != string::npos) {
			currentRace = Race::ZERG;
			currentType = 0;
		} else if (line.find("Terran") != string::npos) {
			currentRace = Race::TERRAN;
			currentType = 0;
		} else if (line.find("#buildings") != string::npos) {
			//starting to read buildings
			currentType = 1;
		} else {
		}
	} else {
		//line does not start with #
		if (line.size() == 0) {
			//reached end
		}
		else {
			//normal line
			parseEntityLine(line);
			/*
			if (currentType == 0) {
				parseUnitLine(line);
			} else {
				parseBuildingLine(line);
			}
			*/
		}
	}
}

void CSVParser::parse() {
	ifstream is(filepath);

	while (is.good()) {
		string s;
		getline(is, s);
		lines.push_back(s);
	}
	
	is.close();
	
	

	for (auto it = lines.begin(); it != lines.end(); ++it) {
		string line = *it;
		parseLine(line);
	}

}

CSVParser::~CSVParser() {
	
}
