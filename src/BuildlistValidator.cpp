#include "../include/BuildlistValidator.h"


BuildlistValidator::BuildlistValidator(Race race, queue<string> buildQueue) : buildQueue(buildQueue), race(race), gasBuildings(0), supply(0.f), supplyMax(0.f) {
	//starting conditions
	switch (race) {
		case TERRAN:
			{
			EntityData& data1 = entityDataMap.at(string("command_center"));
			supply += data1.supplyCost;
			supplyMax += data1.supplyProvided;
			builtTech.insert(string("command_center"));
			
			EntityData& data2 = entityDataMap.at(string("scv"));
			supply += data2.supplyCost * 6;
			supplyMax += data2.supplyProvided * 6;
			builtTech.insert(string("scv"));
			}
			break;
		case PROTOSS:
			{
			EntityData& data3 = entityDataMap.at(string("nexus"));
			supply += data3.supplyCost;
			supplyMax += data3.supplyProvided;
			builtTech.insert(string("nexus"));
			
			EntityData& data4 = entityDataMap.at(string("probe"));
			supply += data4.supplyCost * 6;
			supplyMax += data4.supplyProvided * 6;
			builtTech.insert(string("probe"));
			}
			break;
		case ZERG:
			{
			EntityData& data5 = entityDataMap.at(string("hatchery"));
			supply += data5.supplyCost;
			supplyMax += data5.supplyProvided;
			builtTech.insert(string("hatchery"));
			
			EntityData& data6 = entityDataMap.at(string("drone"));
			supply += data6.supplyCost * 6;
			supplyMax += data6.supplyProvided * 6;
			builtTech.insert(string("drone"));
			
			EntityData& data7 = entityDataMap.at(string("overlord"));
			supply += data7.supplyCost;
			supplyMax += data7.supplyProvided;
			builtTech.insert(string("overlord"));
			}
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
		EntityData& data = entityDataMap.at(s);
		
		if (!dependencyFulfilled(data)) {
			//std::clog << "Buildlist Validator: dependency not fulfilled." << std::endl;
			return false;
		}
		
		if ((data.vespene > 0) && (gasBuildings == 0)) {
			//std::clog << "Buildlist Validator: no gas production, but gas needed." << std::endl;
			return false;
		}
		
		//update supply
		if (data.producedBy.size() == 1) {
			EntityData& producedByData = entityDataMap.at(data.producedBy.at(0));
			if (!producedByData.isBuilding) {
				//producer is a unit: morphing
				supply -= producedByData.supplyCost;
				supplyMax -= producedByData.supplyProvided;
			}
		}
		supply += ((data.name == string("zergling")) ? (data.supplyCost * 2) : data.supplyCost);
		supplyMax += data.supplyProvided;
		//std::clog << "Buildlist Validator: " << data.name << " " << supply << "/" << supplyMax << std::endl;
		
		if (supply > supplyMax) {
			//std::clog << "Buildlist Validator: supply > supplyMax." << std::endl;
			return false;
		}
		
		//requirements met, add new tech
		builtTech.insert(s);
		if ((s == string("refinery")) || (s == string("assimilator")) || (s == string("extractor"))) {
			++gasBuildings;
			if (gasBuildings > 2) {
				//std::clog << "Buildlist Validator: third gas building." << std::endl;
				return false;
			}
		}
	}
	return true;
}

