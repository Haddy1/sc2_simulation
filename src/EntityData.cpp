#include "../include/EntityData.h"

unordered_map<string, EntityData> entityDataMap;

bool entityExists(string s) {
	return entityDataMap.find(s) != entityDataMap.end();
}



unordered_map<int, string> entityNameMap;

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
}
