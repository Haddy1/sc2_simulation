#include "ProtossBuilding.h"

/*
*	ProtossBuildings
*/

ProtossBuilding::ProtossBuilding(int& numEntities, string name, ResourceManager& r) : Building(numEntities, name), busy(false), rm(r), curTime(0), toProduce(nullptr) {
	
}

bool ProtossBuilding::update() {
	if(curTime == getBuildTime()-1) {
		// special case: assimilator
		if(getName().compare("assimilator") == 0) {
			rm.incrementGeysers();
		}
		rm.addSupplyMax(entityData->supplyProvided);
		//techAdd(getName());
		return true;
	}
	++curTime;
	return false;
}

bool ProtossBuilding::isBusy() {
	return busy;
}

bool ProtossBuilding::produceUnit(u_ptr newUnit) {
	toProduce = newUnit;
	busy = true;
	return true;
}

void ProtossBuilding::finishUnit() {
	toProduce = nullptr;
	busy = false;
}


u_ptr ProtossBuilding::getProducedUnit() {
	return toProduce;
}

/*
*	Nexus class to manage energy/chronoboost
*/
Nexus::Nexus(int& numEntities, string name, ResourceManager& r) : ProtossBuilding(numEntities, name, r), energy(0) {
	rm.addSupplyMax(entityData->supplyProvided);
	//techAdd(getName());
}

bool Nexus::update() {
	energy += FixedPoint(0.5625);
	energy = (energy > 100) ? 100 : energy;
	return false;
}

FixedPoint Nexus::getEnergy() {
	return energy;
}

void Nexus::consumeEnergy() {
	energy -= 25;
}
