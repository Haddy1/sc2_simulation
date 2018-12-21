#include "../include/BuildlistValidator.h"


BuildlistValidator::BuildlistValidator(Race race, queue<string> buildQueue) : buildQueue(buildQueue), race(race), gasBuildingBuilt(false) {
	//starting conditions
	switch (race) {
		case TERRAN:
			builtTech.insert(string("scv"));
			builtTech.insert(string("command_center"));
			break;
		case PROTOSS:
			builtTech.insert(string("probe"));
			builtTech.insert(string("nexus"));
			break;
		case ZERG:
			builtTech.insert(string("hatchery"));
			builtTech.insert(string("drone"));
			builtTech.insert(string("overlord"));
			break;
		default:
			break;
	}
}

bool BuildlistValidator::dependencyFulfilled(EntityData& e) {
	if (e.dependencies.size() == 0) {
		return true;
	}
	for (string& dep : e.dependencies) {
		if (builtTech.find(dep) != builtTech.end()) {
			return true;
		}
	}
	return false;
}
	
bool BuildlistValidator::validate() {
	//Check if each entry has its dependencies fulfilled by previous entries
	while (!buildQueue.empty()) {
		string s = buildQueue.front();
		buildQueue.pop();
		EntityData data = entityDataMap.at(s);
		if (!dependencyFulfilled(data)) {
			return false;
		}
		
		if ((data.vespene > 0) && (!gasBuildingBuilt)) {
			return false;
		}
		builtTech.insert(s);
		if ((s == string("refinery")) || (s == string("assimilator")) || (s == string("extractor"))) {
			gasBuildingBuilt = true;
		}
	}
	return true;
}

