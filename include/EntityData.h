#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>

#include "Race.h"

using std::unordered_map;
using std::string;
using std::vector;


enum EntityType {
	none = 0,
	//TODO: add terran and protoss units/buildings
	
	//ZERG
	zerg_start = 100,
	overlord = zerg_start,
	overseer,
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
	//buildings
	hatchery,
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
	spire,
	zerg_end,
	
	
	//TERRAN
	terran_start = 200,
	scv = terran_start,
	marine,
	marauder,
	reaper,
	ghost,
	hellion,
	siege_tank,
	thor,
	medivac,
	viking,
	raven,
	banshee,
	battlecruiser,
	//buildings
	command_center,
	orbital_command,
	planetary_fortress,
	refinery,
	engineering_bay,
	missile_turret,
	sensor_tower,
	barracks,
	factory,
	armory,
	bunker,
	ghost_academy,
	starport,
	fusion_core,
	supply_depot,
	barracks_with_reactor,
	barracks_with_tech_lab,
	starport_with_reactor,
	starport_with_tech_lab,
	factory_with_reactor,
	factory_with_tech_lab,
	terran_end,
	
	
	//PROTOSS
	protoss_start = 300,
	probe = protoss_start,
	zealot,
	stalker,
	sentry,
	warp_prism,
	immortal,
	observer,
	colossus,
	high_templar,
	dark_templar,
	mothership,
	phoenix,
	void_ray,
	carrier,
	//buildings
	nexus,
	pylon,
	gateway,
	cybernetics_core,
	robotics_facility,
	robotics_bay,
	twilight_council,
	templar_archives,
	dark_shrine,
	stargate,
	fleet_beacon,
	assimilator,
	forge,
	photon_cannon,
	protoss_end
	
	
};


struct EntityData {
	EntityType type;
	string name;
	int minerals;
	int vespene;
	int buildTime;
	float supplyCost; //can be 0.5 for zerglings
	int supplyProvided;
	int startEnergy;
	int maxEnergy;
	Race race;
	vector<EntityType> producedBy;
	vector<EntityType> dependencies;
	bool isBuilding;

};

extern unordered_map<EntityType, EntityData> entityDataMap;

bool entityExists(string s);

bool entityExists(EntityType t);






extern unordered_map<EntityType, string> entityNameMap;
extern unordered_map<string, EntityType> nameEntityMap;

void initEntityNameMap();


