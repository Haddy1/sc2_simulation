#include "../include/EntityData.h"
#include "../include/Race.h"
#include "../include/ResourceManager.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>
#include <utility>

#pragma once

using std::cout;
using std::endl;
using std::vector;
using std::make_pair;
using std::pair;

class EventEntry {
public:
	EventEntry(string type, string name) : val(make_pair(type, name)) {
		
	}
	
	string first() const {
		return val.first;
	}
	string second() const {
		return val.second;
	}
	
	virtual string getID() const {return "";}
	virtual string getTarget() const {return "";}
	
	virtual bool valid() const {
		const char* type = first().c_str();
		string name = second();
		return (strcmp(type, "build-start") == 0 || strcmp(type, "build-end") == 0) && (entityDataMap.find(name) != entityDataMap.end());
	}
private:
	pair<string, string> val;
};

class AbilityEntry : public EventEntry {
public:
	AbilityEntry(string type, string name, string id = "", string target = "") : EventEntry(type, name), triggeredBy(id), targetBuilding(target) {
		
	}
	
	virtual string getID() const {return triggeredBy;}
	virtual string getTarget() const {return targetBuilding;}
	
	bool valid() const {
		const char* type = first().c_str();
		const char* name = second().c_str();
		bool validSpecial = strcmp(type, "special") == 0 && (strcmp(name, "mule") == 0 || strcmp(name, "chronoboost") == 0 || strcmp(name, "injectlarvae") == 0);
		bool validID = (!triggeredBy.empty()) ? entityDataMap.find(triggeredBy.substr(0, triggeredBy.find("_", 0))) != entityDataMap.end() : true;
		bool validTarget = (!targetBuilding.empty()) ? entityDataMap.find(targetBuilding) != entityDataMap.end() : true;
		return validSpecial && validID && validTarget;
	}
private:
	string triggeredBy;
	string targetBuilding;
};

class JsonLogger {
public:
	JsonLogger(Race, ResourceManager&, bool);
	JsonLogger(Race, ResourceManager&, bool, std::string);
	~JsonLogger();
	
	void printSetup(vector<pair<string, vector<int>>>);
	void printMessage(int, vector<EventEntry*>);
	
private:
	Race race;
	ResourceManager& rm;
	bool validBuildlist;
	
	std::string path;
	char ws = '\t'; // changeable whitespace character
	
	std::ofstream out;
	std::streambuf* coutbuf;
	
	void printBeginning();
	void redirect();
	void undo_redirect();
};
