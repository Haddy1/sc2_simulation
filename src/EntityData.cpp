#include "../include/EntityData.h"

unordered_map<EntityType, EntityData> entityDataMap;

bool entityExists(string s) {
	return nameEntityMap.find(s) != nameEntityMap.end();
}

bool entityExists(EntityType t) {
	return entityNameMap.find(t) != entityNameMap.end();
}



unordered_map<EntityType, string> entityNameMap;
unordered_map<string, EntityType> nameEntityMap;

void initEntityNameMap() {
	entityNameMap.emplace(overlord, string("overlord"));
	entityNameMap.emplace(overseer, string("overseer"));
	entityNameMap.emplace(hydralisk, string("hydralisk"));
	entityNameMap.emplace(drone, string("drone"));
	entityNameMap.emplace(queen, string("queen"));
	entityNameMap.emplace(larva, string("larva"));
	entityNameMap.emplace(zergling, string("zergling"));
	entityNameMap.emplace(baneling, string("baneling"));
	entityNameMap.emplace(roach, string("roach"));
	entityNameMap.emplace(infestor, string("infestor"));
	entityNameMap.emplace(nydus_worm, string("nydus_worm"));
	entityNameMap.emplace(ultralisk, string("ultralisk"));
	entityNameMap.emplace(brood_lord, string("brood_lord"));
	entityNameMap.emplace(corruptor, string("corruptor"));
	entityNameMap.emplace(mutalisk, string("mutalisk"));
	
	entityNameMap.emplace(hatchery, string("hatchery"));
	entityNameMap.emplace(evolution_chamber, string("evolution_chamber"));
	entityNameMap.emplace(spore_crawler, string("spore_crawler"));
	entityNameMap.emplace(lair, string("lair"));
	entityNameMap.emplace(spawning_pool, string("spawning_pool"));
	entityNameMap.emplace(extractor, string("extractor"));
	entityNameMap.emplace(spine_crawler, string("spine_crawler"));
	entityNameMap.emplace(roach_warren, string("roach_warren"));
	entityNameMap.emplace(baneling_nest, string("baneling_nest"));
	entityNameMap.emplace(hydralisk_den, string("hydralisk_den"));
	entityNameMap.emplace(infestation_pit, string("infestation_pit"));
	entityNameMap.emplace(hive, string("hive"));
	entityNameMap.emplace(nydus_network, string("nydus_network"));
	entityNameMap.emplace(ultralisk_cavern, string("ultralisk_cavern"));
	entityNameMap.emplace(greater_spire, string("greater_spire"));
	entityNameMap.emplace(spire, string("spire"));
	
	
	
	
	entityNameMap.emplace(scv,string("scv"));
	entityNameMap.emplace(marine,string("marine"));
	entityNameMap.emplace(marauder,string("marauder"));
	entityNameMap.emplace(reaper,string("eaper"));
	entityNameMap.emplace(ghost,string("ghost"));
	entityNameMap.emplace(hellion,string("hellion"));
	entityNameMap.emplace(siege_tank,string("siege_tank"));
	entityNameMap.emplace(thor,string("thor"));
	entityNameMap.emplace(medivac,string("medivac"));
	entityNameMap.emplace(viking,string("viking"));
	entityNameMap.emplace(raven,string("raven"));
	entityNameMap.emplace(banshee,string("banshee"));
	entityNameMap.emplace(battlecruiser,string("battlecruiser"));
	
	entityNameMap.emplace(command_center,string("command_center"));
	entityNameMap.emplace(orbital_command,string("orbital_command"));
	entityNameMap.emplace(planetary_fortress,string("planetary_fortress"));
	entityNameMap.emplace(refinery,string("refinery"));
	entityNameMap.emplace(engineering_bay,string("engineering_bay"));
	entityNameMap.emplace(missile_turret,string("missile_turret"));
	entityNameMap.emplace(sensor_tower,string("sensor_tower"));
	entityNameMap.emplace(barracks,string("barracks"));
	entityNameMap.emplace(factory,string("factory"));
	entityNameMap.emplace(armory,string("armory"));
	entityNameMap.emplace(bunker,string("bunker"));
	entityNameMap.emplace(ghost_academy,string("ghost_academy"));
	entityNameMap.emplace(starport,string("starport"));
	entityNameMap.emplace(fusion_core,string("fusion_core"));
	entityNameMap.emplace(supply_depot,string("supply_depot"));
	entityNameMap.emplace(barracks_with_reactor,string("barracks_with_reactor"));
	entityNameMap.emplace(barracks_with_tech_lab,string("barracks_with_tech_lab"));
	entityNameMap.emplace(starport_with_reactor,string("starport_with_reactor"));
	entityNameMap.emplace(starport_with_tech_lab,string("starport_with_tech_lab"));
	entityNameMap.emplace(factory_with_reactor,string("factory_with_reactor"));
	entityNameMap.emplace(factory_with_tech_lab,string("factory_with_tech_lab"));
	
	
	
	
	entityNameMap.emplace(probe,string("probe"));
	entityNameMap.emplace(zealot,string("zealot"));
	entityNameMap.emplace(stalker,string("stalker"));
	entityNameMap.emplace(sentry,string("sentry"));
	entityNameMap.emplace(warp_prism,string("warp_prism"));
	entityNameMap.emplace(immortal,string("immortal"));
	entityNameMap.emplace(observer,string("observer"));
	entityNameMap.emplace(colossus,string("colossus"));
	entityNameMap.emplace(high_templar,string("high_templar"));
	entityNameMap.emplace(dark_templar,string("dark_templar"));
	entityNameMap.emplace(mothership,string("mothership"));
	entityNameMap.emplace(phoenix,string("phoenix"));
	entityNameMap.emplace(void_ray,string("void_ray"));
	entityNameMap.emplace(carrier,string("carrier"));
	
	entityNameMap.emplace(nexus,string("nexus"));
	entityNameMap.emplace(pylon,string("pylon"));
	entityNameMap.emplace(gateway,string("gateway"));
	entityNameMap.emplace(cybernetics_core,string("cybernetics_core"));
	entityNameMap.emplace(robotics_facility,string("robotics_facility"));
	entityNameMap.emplace(robotics_bay,string("robotics_bay"));
	entityNameMap.emplace(twilight_council,string("twilight_council"));
	entityNameMap.emplace(templar_archives,string("templar_archives"));
	entityNameMap.emplace(dark_shrine,string("dark_shrine"));
	entityNameMap.emplace(stargate,string("stargate"));
	entityNameMap.emplace(fleet_beacon,string("fleet_beacon"));
	entityNameMap.emplace(assimilator,string("assimilator"));
	entityNameMap.emplace(forge,string("forge"));
	entityNameMap.emplace(photon_cannon,string("photon_cannon"));
	
	
	
	for (auto it = entityNameMap.begin(); it != entityNameMap.end(); ++it) {
		nameEntityMap.emplace(it->second, it->first);
	}
}
