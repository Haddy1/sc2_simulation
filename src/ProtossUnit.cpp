#include "ProtossUnit.h"

/*
*	ProtossUnits
*/

ProtossUnit::ProtossUnit(int& numEntities, string name, b_ptr prod, ResourceManager& r) : Unit(numEntities, name), curTime(0), producer(prod), rm(r) {
	
}


b_ptr ProtossUnit::getProducer() {
	return producer;
}

bool ProtossUnit::update(bool finish) {
	++curTime;
	if(curTime >= getBuildTime() && finish) { // dont finish in chonoboost_handler
		// special case: probe
		if(getName().compare("probe") == 0) {
			rm.incrementMineralWorkers(); // redistribution occurs before log
		}
		producer->finishUnit();
		return true;
	}
	return false;
}
