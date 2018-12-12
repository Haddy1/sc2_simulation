#include "../include/BuildlistValidator.h"


BuildlistValidator::BuildlistValidator(Race race, queue<string> buildQueue) : buildQueue(buildQueue), race(race) {
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
		if (!dependencyFulfilled(entityDataMap.at(s))) {
			return false;
		}
		builtTech.insert(s);
	}
	return true;
}

