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
	float supplyCost; //can be 0.5 for zerglings
	int supplyProvided;
	int startEnergy;
	int maxEnergy;
	Race race;
	vector<string> producedBy;
	vector<string> dependencies;
	bool isBuilding;

	friend std::ostream& operator<<(std::ostream& os, const EntityData& d) {
		os << d.name << "," << d.minerals << "," << d.vespene << "," << d.buildTime << "," << d.supplyCost << "," << d.supplyProvided << "," << d.startEnergy << "," << d.maxEnergy << "," << d.race << ",";
		os << " producedBy: ";
		for (string s : d.producedBy) os << s << "/";
		os << ",";
		os << " dependencies: ";
		for (string s : d.dependencies) os << s << "/";
		return os;
	}
};

extern unordered_map<string, EntityData> entityDataMap;

bool entityExists(string s);




enum EntityType {
	none = 0,
	//todo optional: add terran and protoss units/buildings
	
	overlord = 38, //values oriented at .csv file row number (starting at 1 not 0)
	overseer, //has value 39
	hydralisk,
	drone,
	queen,
	larva,
	zergling,
	baneling,
	roach,
	infestor,
	nydus_worm,
	ultralisk,
	brood_lord,
	corruptor,
	mutalisk,
	
	hatchery = 54,
	evolution_chamber,
	spore_crawler,
	lair,
	spawning_pool,
	extractor,
	spine_crawler,
	roach_warren,
	baneling_nest,
	hydralisk_den,
	infestation_pit,
	hive,
	nydus_network,
	ultralisk_cavern,
	greater_spire,
	spire
};

extern unordered_map<int, string> entityNameMap;

void initEntityNameMap();

