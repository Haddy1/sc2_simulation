#include "ProtossBuilding.h"

/*
*	ProtossBuildings
*/
ProtossBuilding::ProtossBuilding(int& numEntities, string name, ResourceManager& r) : Building(numEntities, name), busy(false), r(r), curTime(0) {
	
}

bool ProtossBuilding::update() {
	if(curTime == getBuildTime()-1) {
		r.addSupplyMax(entityData->supplyProvided);
		techAdd(getName());
		return true;
	}
	++curTime;
	return false;
}

/*
*	concrete Nexus class to manage energy/chronoboost
*/
Nexus::Nexus(int& numEntities, string name, ResourceManager& r) : ProtossBuilding(numEntities, name, r), energy(0) {
	r.addSupplyMax(entityData->supplyProvided);
	techAdd(getName());
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

/*
*	upgradable to warpgate
*/
/*
Gateway::Gateway(int& numEntities, string name, ResourceManager& r) : ProtossBuilding(numEntities, name, r) {
	
}
*/
