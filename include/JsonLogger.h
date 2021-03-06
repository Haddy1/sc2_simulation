#include "../include/EntityData.h"
#include "../include/Race.h"
#include "../include/ResourceManager.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>
#include <utility>
#include <string>
#include <memory>

#pragma once

using std::cout;
using std::endl;
using std::vector;
using std::make_pair;
using std::pair;
using std::to_string;
using std::shared_ptr;
using std::make_shared;

/*
*	Entry classes to log events
*/
class EventEntry {
public:
	EventEntry(string type, string name, string producerID = "", vector<string> producedIDs = vector<string>()) : val(make_pair(type, name)), producerID(producerID), producedIDs(producedIDs) {
		
	}
	
	EventEntry(string type, string name, int producerID_) : val(make_pair(type, name)) {
		producerID = to_string(producerID_);
	}
	
	EventEntry(string type, string name, int producerID_, int producedID_) : val(make_pair(type, name)) {
		producerID = to_string(producerID_);
		producedIDs.push_back(to_string(producedID_));
	}
	
	virtual ~EventEntry() {
		
	}
	
	string first() const {
		return val.first;
	}
	string second() const {
		return val.second;
	}
	
	string getProducerID() const {
		return producerID;
	}
	
	vector<string> getProducedIDs() const {
		return producedIDs;
	}
	
	virtual string getID() const {return "";}
	virtual string getTarget() const {return "";}
	
	virtual bool isAbilityEntry() const {
		return false;
	}
	
	virtual bool valid() const {
		const char* type = first().c_str();
		string name = second();
		return (strcmp(type, "build-start") == 0 || strcmp(type, "build-end") == 0); // && (entityDataMap.find(name) != entityDataMap.end());
	}
private:
	pair<string, string> val;
	string producerID;
	vector<string> producedIDs;
};

class AbilityEntry : public EventEntry {
public:
	AbilityEntry(string type, string name, string id, string target = "") : EventEntry(type, name), triggeredBy(id), targetBuilding(target) {
		
	}
	
	AbilityEntry(string type, string name, int id, int target) : EventEntry(type, name) {
		triggeredBy = to_string(id);
		targetBuilding = to_string(target);
	}
	
	~AbilityEntry() {
		
	}
	
	virtual string getID() const {return triggeredBy;}
	virtual string getTarget() const {return targetBuilding;}
	
	bool isAbilityEntry() const {
		return true;
	}
	
	bool valid() const {
		const char* type = first().c_str();
		const char* name = second().c_str();
		bool validSpecial = strcmp(type, "special") == 0 && (strcmp(name, "mule") == 0 || strcmp(name, "chronoboost") == 0 || strcmp(name, "injectlarvae") == 0);
		//bool validID = (!triggeredBy.empty()) ? entityDataMap.find(triggeredBy.substr(0, triggeredBy.find("_", 0))) != entityDataMap.end() : true;
		//bool validTarget = (!targetBuilding.empty()) ? entityDataMap.find(targetBuilding) != entityDataMap.end() : true;
		//return validSpecial && validID && validTarget;
		return validSpecial;
	}
private:
	string triggeredBy;
	string targetBuilding;
};

/*
*	Inline functions to create smart pointers for the events
*/
inline shared_ptr<EventEntry> create_event_ptr(string type, string name, string producerID = "", vector<string> producedIDs = vector<string>()) {
	return make_shared<EventEntry>(EventEntry(type, name, producerID, producedIDs));
}
	
inline shared_ptr<EventEntry> create_event_ptr(string type, string name, int producerID_) {
	return make_shared<EventEntry>(EventEntry(type, name, producerID_));
}
	
inline shared_ptr<EventEntry> create_event_ptr(string type, string name, int producerID_, int producedID_) {
	return make_shared<EventEntry>(EventEntry(type, name, producerID_, producedID_));
}

inline shared_ptr<AbilityEntry> create_ability_ptr(string type, string name, string id, string target = "") {
	return make_shared<AbilityEntry>(AbilityEntry(type, name, id, target)); 
}
	
inline shared_ptr<AbilityEntry> create_ability_ptr(string type, string name, int id, int target) {
	return make_shared<AbilityEntry>(AbilityEntry(type, name, id, target)); 
}

/*
*	Logger class
*/
class JsonLogger {
public:
	JsonLogger(Race, ResourceManager&, bool);
	JsonLogger(Race, ResourceManager&, bool, std::string);
	~JsonLogger();
	
	void printSetup(vector<pair<string, vector<int>>>&);
	//void printMessage(int, vector<EventEntry*>&);
	void printMessage(int, vector<shared_ptr<EventEntry>>&);
	void printMessageStart();
	
private:
	Race race;
	ResourceManager& rm;
	bool validBuildlist;
	bool firstMsg;
	
	std::string path;
	char ws = '\t'; // changeable whitespace character
	
	std::ofstream out;
	std::streambuf* coutbuf;
	
	void printBeginning();
	void redirect();
	void undo_redirect();
};
