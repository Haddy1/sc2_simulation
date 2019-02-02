#include "ProtossUnit.h"

/*
*	ProtossUnits
*/

ProtossUnit::ProtossUnit(int& numEntities, EntityType type, b_ptr prod, ResourceManager& r) : Unit(numEntities, type), curTime(0.), producer(prod), rm(r) {
	
}


b_ptr ProtossUnit::getProducer() {
	return producer;
}

bool ProtossUnit::update(double rate, bool finish) {
	curTime += rate;
	if(curTime >= getBuildTime() && finish) { // dont finish in chonoboost_handler
		// special case: probe
		if(getType() == probe) {
			rm.incrementMineralWorkers(); // redistribution occurs before log
		}
		producer->finishUnit();
		return true;
	}
	return false;
}
