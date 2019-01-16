#include "../include/BuildlistValidator.h"


BuildlistValidator::BuildlistValidator(Race race) : race(race), gasBuildings(0), supply(0.f), supplyMax(0.f) {
	init();
}

BuildlistValidator::BuildlistValidator(Race race, queue<EntityType> buildQueue) : buildQueue(buildQueue), race(race), gasBuildings(0), supply(0.f), supplyMax(0.f) {
	init();
}

void BuildlistValidator::init() {
	//starting conditions
	switch (race) {
		case TERRAN:
			{
			EntityData& data1 = entityDataMap.at(command_center);
			supply += data1.supplyCost;
			supplyMax += data1.supplyProvided;
			tech.add(command_center);
			
			EntityData& data2 = entityDataMap.at(scv);
			supply += data2.supplyCost * 6;
			supplyMax += data2.supplyProvided * 6;
			tech.add(scv);
			}
			break;
		case PROTOSS:
			{
			EntityData& data3 = entityDataMap.at(nexus);
			supply += data3.supplyCost;
			supplyMax += data3.supplyProvided;
			tech.add(nexus);
			
			EntityData& data4 = entityDataMap.at(probe);
			supply += data4.supplyCost * 6;
			supplyMax += data4.supplyProvided * 6;
			tech.add(probe);
			}
			break;
		case ZERG:
			{
			EntityData& data5 = entityDataMap.at(hatchery);
			supply += data5.supplyCost;
			supplyMax += data5.supplyProvided;
			tech.add(hatchery);
			
			EntityData& data6 = entityDataMap.at(drone);
			supply += data6.supplyCost * 6;
			supplyMax += data6.supplyProvided * 6;
			tech.add(drone);
			
			EntityData& data7 = entityDataMap.at(overlord);
			supply += data7.supplyCost;
			supplyMax += data7.supplyProvided;
			tech.add(overlord);
			}
			break;
		default:
			break;
	}
}

bool BuildlistValidator::validate() {
	//Check if each entry has its dependencies fulfilled by previous entries
	while (!buildQueue.empty()) {
		EntityType type = buildQueue.front();
		buildQueue.pop();
		if (!validateNext(type)) {
			return false;
		}
	}
	return true;
}

bool BuildlistValidator::validateNext(EntityType t) {
	EntityData& data = entityDataMap.at(t);
	
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
	supply += ((data.type == zergling) ? (data.supplyCost * 2) : data.supplyCost);
	supplyMax += data.supplyProvided;
		
	if (supply > supplyMax) {
		return false;
	}
	
	//requirements met, add new tech
	tech.add(t);
	if ((t == refinery) || (t == assimilator) || (t == extractor)) {
		++gasBuildings;
		if (gasBuildings > 2) {
			return false;
		}
	}
	
	return true;
}

bool BuildlistValidator::checkNext(EntityType t) {
	EntityData& data = entityDataMap.at(t);
	
	if (!tech.dependencyFulfilled(data)) {
		return false;
	}
	
	if ((data.vespene > 0) && (gasBuildings == 0)) {
		return false;
	}
	
	float supplyChange = ((data.type == zergling) ? (data.supplyCost * 2) : data.supplyCost);
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
		if ((t == refinery) || (t == assimilator) || (t == extractor)) {
			return false;
		}
	}
	
	return true;
}

