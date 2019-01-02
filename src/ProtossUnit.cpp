#include "ProtossUnit.h"

/*
*	ProtossUnits
*/
ProtossUnit::ProtossUnit(int& numEntities, string name, shared_ptr<ProtossBuilding> prod) : Unit(numEntities, name), curTime(0), producer(prod) {
	
}

shared_ptr<ProtossBuilding> ProtossUnit::getProducer() {
	return producer;
}

bool ProtossUnit::update() {
	if(curTime == getBuildTime()-1) {
		return true;
	}
	++curTime;
	return false;
}