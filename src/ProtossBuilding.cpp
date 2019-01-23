#include "ProtossBuilding.h"

/*
*	ProtossBuildings
*/

ProtossBuilding::ProtossBuilding(int& numEntities, string name, ResourceManager& r, Tech& t) : Building(numEntities, name), busy(false), rm(r), tech(t), curTime(0), toProduce(nullptr) {
	
}

bool ProtossBuilding::update() {
	if(curTime == getBuildTime()-1) {
		// special case: assimilator
		if(getName().compare("assimilator") == 0) {
			rm.incrementGeysers();
		}
		rm.addSupplyMax(entityData->supplyProvided);
		tech.add(getName());
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
Nexus::Nexus(int& numEntities, string name, ResourceManager& r, Tech& t) : ProtossBuilding(numEntities, name, r, t), energy(0) {
	
}

void Nexus::updateEnergy() {
	energy += FixedPoint(0.5625);
	energy = (energy > 100) ? 100 : energy;
}

FixedPoint Nexus::getEnergy() {
	return energy;
}

void Nexus::consumeEnergy() {
	energy -= 25;
}
