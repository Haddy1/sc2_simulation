#include "../include/EntityData.h"

unordered_map<string, EntityData> entityDataMap;

bool entityExists(string s) {
	return entityDataMap.find(s) != entityDataMap.end();
}



unordered_map<int, string> entityNameMap;
//TODO
void initEntityNameMap() {
	entityNameMap.emplace(EntityType::overlord, string("overlord"));
	entityNameMap.emplace(overseer, string("overseer"));
}
