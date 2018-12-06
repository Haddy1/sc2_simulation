#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>

#include "Race.h"

using std::unordered_map;
using std::string;
using std::vector;

struct EntityData {
	string name;
	int minerals;
	int vespene;
	int buildTime;
	int supplyCost;
	int supplyProvided;
	int startEnergy;
	int maxEnergy;
	Race race;
	vector<string> producedBy;
	vector<string> dependencies;
	bool isBuilding;

	friend std::ostream& operator<<(std::ostream& os, const EntityData& d) {
		os << d.name << "," << d.minerals << "," << d.vespene << "," << d.buildTime << "," << d.supplyCost << "," << d.supplyProvided << ","<< d.startEnergy << "," << d.maxEnergy << "," << d.race << ",";
		os << " producedBy: ";
		for (string s : d.producedBy) os << s << "/";
		os << ",";
		os << " dependencies: ";
		for (string s : d.dependencies) os << s << "/";
		return os;
	}
};

extern unordered_map<string, EntityData> entityDataMap;

inline bool entityExists(string s) {
	return entityDataMap.find(s) != entityDataMap.end();
}
