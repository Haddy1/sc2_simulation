#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>

using std::unordered_map;
using std::string;
using std::vector;

struct UnitData {
	string name;
	int minerals;
	int vespene;
	int buildTime;
	int supplyCost;
	int supplyProvided;
	int startEnergy;
	int maxEnergy;
	int race;
	vector<string> producedBy;
	vector<string> dependencies;

	friend std::ostream& operator<<(std::ostream& os, const UnitData& d) {
		os << d.name << "," << d.minerals << "," << d.vespene << "," << d.buildTime << "," << d.supplyCost << "," << d.supplyProvided << ","<< d.startEnergy << "," << d.maxEnergy << "," << d.race << ",";
		os << " producedBy: size: " << d.producedBy.size() << ": ";
		for (string s : d.producedBy) os << s << "/";
		os << ",";
		os << " dependencies: size: " << d.dependencies.size() << ": ";
		for (string s : d.dependencies) os << s << "/";
		return os;
	}
};

extern unordered_map<string, UnitData> unitDataMap;
