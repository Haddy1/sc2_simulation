#include "../include/BuildlistValidator.h"


BuildlistValidator::BuildlistValidator(Race race) : race(race), gasBuildings(0), supply(0.f), supplyMax(0.f) {
	//starting conditions
	switch (race) {
		case TERRAN:
			{
			EntityData& data1 = entityDataMap.at(string("command_center"));
			supply += data1.supplyCost;
			supplyMax += data1.supplyProvided;
			tech.add(string("command_center"));
			
			EntityData& data2 = entityDataMap.at(string("scv"));
			supply += data2.supplyCost * 6;
			supplyMax += data2.supplyProvided * 6;
			tech.add(string("scv"));
			}
			break;
		case PROTOSS:
			{
			EntityData& data3 = entityDataMap.at(string("nexus"));
			supply += data3.supplyCost;
			supplyMax += data3.supplyProvided;
			tech.add(string("nexus"));
			
			EntityData& data4 = entityDataMap.at(string("probe"));
			supply += data4.supplyCost * 6;
			supplyMax += data4.supplyProvided * 6;
			tech.add(string("probe"));
			}
			break;
		case ZERG:
			{
			EntityData& data5 = entityDataMap.at(string("hatchery"));
			supply += data5.supplyCost;
			supplyMax += data5.supplyProvided;
			tech.add(string("hatchery"));
			
			EntityData& data6 = entityDataMap.at(string("drone"));
			supply += data6.supplyCost * 6;
			supplyMax += data6.supplyProvided * 6;
			tech.add(string("drone"));
			
			EntityData& data7 = entityDataMap.at(string("overlord"));
			supply += data7.supplyCost;
			supplyMax += data7.supplyProvided;
			tech.add(string("overlord"));
			}
			break;
		default:
			break;
	}
}

BuildlistValidator::BuildlistValidator(Race race, queue<string> buildQueue) : buildQueue(buildQueue), race(race), gasBuildings(0), supply(0.f), supplyMax(0.f) {
	//starting conditions
	switch (race) {
		case TERRAN:
			{
			EntityData& data1 = entityDataMap.at(string("command_center"));
			supply += data1.supplyCost;
			supplyMax += data1.supplyProvided;
			tech.add(string("command_center"));
			
			EntityData& data2 = entityDataMap.at(string("scv"));
			supply += data2.supplyCost * 6;
			supplyMax += data2.supplyProvided * 6;
			tech.add(string("scv"));
			}
			break;
		case PROTOSS:
			{
			EntityData& data3 = entityDataMap.at(string("nexus"));
			supply += data3.supplyCost;
			supplyMax += data3.supplyProvided;
			tech.add(string("nexus"));
			
			EntityData& data4 = entityDataMap.at(string("probe"));
			supply += data4.supplyCost * 6;
			supplyMax += data4.supplyProvided * 6;
			tech.add(string("probe"));
			}
			break;
		case ZERG:
			{
			EntityData& data5 = entityDataMap.at(string("hatchery"));
			supply += data5.supplyCost;
			supplyMax += data5.supplyProvided;
			tech.add(string("hatchery"));
			
			EntityData& data6 = entityDataMap.at(string("drone"));
			supply += data6.supplyCost * 6;
			supplyMax += data6.supplyProvided * 6;
			tech.add(string("drone"));
			
			EntityData& data7 = entityDataMap.at(string("overlord"));
			supply += data7.supplyCost;
			supplyMax += data7.supplyProvided;
			tech.add(string("overlord"));
			}
			break;
		default:
			break;
	}
}

bool BuildlistValidator::validate() {
	//Check if each entry has its dependencies fulfilled by previous entries
	while (!buildQueue.empty()) {
		string s = buildQueue.front();
		buildQueue.pop();
		if (!validateNext(s)) {
			return false;
		}
	}
	return true;
}

bool BuildlistValidator::validateNext(string s) {
	EntityData& data = entityDataMap.at(s);
	
	if (!tech.dependencyFulfilled(data)) {
		return false;
	}
	
	if ((data.vespene > 0) && (gasBuildings == 0)) {
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
		
	if (supply > supplyMax) {
		return false;
	}
	
	//requirements met, add new tech
	tech.add(s);
	if ((s == string("refinery")) || (s == string("assimilator")) || (s == string("extractor"))) {
		++gasBuildings;
		if (gasBuildings > 2) {
			return false;
		}
	}
	
	return true;
}

bool BuildlistValidator::checkNext(string s) {
	EntityData& data = entityDataMap.at(s);
	
	if (!tech.dependencyFulfilled(data)) {
		return false;
	}
	
	if ((data.vespene > 0) && (gasBuildings == 0)) {
		return false;
	}
	
	float supplyChange = ((data.name == string("zergling")) ? (data.supplyCost * 2) : data.supplyCost);
	float supplyMaxChange = data.supplyProvided;
	
	if (data.producedBy.size() == 1) {
		EntityData& producedByData = entityDataMap.at(data.producedBy.at(0));
		if (!producedByData.isBuilding) {
			//producer is a unit: morphing
			supplyChange -= producedByData.supplyCost;
			supplyMaxChange -= producedByData.supplyProvided;
		}
	}
	
		
	if ((supply + supplyChange) > (supplyMax + supplyMaxChange)) {
		return false;
	}
	
	if (gasBuildings == 2) {
		if ((s == string("refinery")) || (s == string("assimilator")) || (s == string("extractor"))) {
			return false;
		}
	}
	
	return true;
}

