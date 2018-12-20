#include "../include/EntityData.h"
#include "../include/Race.h"
#include "../include/ResourceManager.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>
#include <utility>
#include <string>

#pragma once

using std::cout;
using std::endl;
using std::vector;
using std::make_pair;
using std::pair;
using std::to_string;

struct BuildStartEntry {
public:
	BuildStartEntry() {}

	BuildStartEntry(string name, string producerID = "") : name(name), producerID(producerID) {
		
	}
	
	BuildStartEntry(string name, int producerID_) : name(name), producerID(to_string(producerID_)) {
		//producerID = to_string(producerID_);
	}
	
	string name;
	string producerID;
};

struct BuildEndEntry {
public:
	BuildEndEntry() {}
	
	BuildEndEntry(string name, string producerID_ = "") : name(name), producerID(producerID_) {
		
	}
	
	BuildEndEntry(string name, string producerID, vector<string> producedIDs) : producerID(producerID), producedIDs(producedIDs) {
		
	}
	
	BuildEndEntry(string name, int producerID_) {
		producerID = to_string(producerID_);
	}
	
	BuildEndEntry(string name, int producerID_, int producedID_) {
		producerID = to_string(producerID_);
		producedIDs.push_back(to_string(producedID_));
	}
	
	string name;
	string producerID;
	vector<string> producedIDs;
};

struct AbilityEntry {
public:
	AbilityEntry() {}
	
	AbilityEntry(string name, string trigger, string target = "") : name(name), triggeredBy(trigger), targetBuilding(target) {
		
	}
	
	AbilityEntry(string name, int trigger_) : name(name) {
		triggeredBy = to_string(trigger_);
		targetBuilding = "";
	}
	
	AbilityEntry(string name, int trigger_, int target_) : name(name) {
		triggeredBy = to_string(trigger_);
		targetBuilding = to_string(target_);
	}

	string name;
	string triggeredBy;
	string targetBuilding;
};



class JsonLoggerV2 {
public:
	JsonLoggerV2(Race, ResourceManager&, bool);
	~JsonLoggerV2();
	
	void printBeginning();
	void printSetup(vector<pair<string, vector<int>>>&);
	void printMessage(int);
	void addBuildstart(BuildStartEntry b);
	void addBuildend(BuildEndEntry b);
	void addAbility(AbilityEntry a);
	
private:
	Race race;
	ResourceManager& rm;
	bool validBuildlist;
	
	char ws = '\t'; // changeable whitespace character
	char quote = '\"';
	
	BuildStartEntry buildstartEvent;
	vector<BuildEndEntry> buildendEvents;
	AbilityEntry abilityEvent;
	
	bool hasBuildstart;
	bool hasAbility;
};
