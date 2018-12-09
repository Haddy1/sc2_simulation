#include "../include/ZergUnit.h"

/*
 * Generic Unit that cant morph
 */
ZergUnit::ZergUnit(string name, ResourceManager& r) : Unit(name), r(r) {

}


/*
 * Larva
 */
ZergLarva::ZergLarva(string name, ResourceManager& r, string morphingTo) : ZergUnit(name, r), morphingToData(entityDataMap.at(morphingTo)), morphProgress(0) {

}

ZergLarva::ZergLarva(string name, ResourceManager& r, EntityData& morphingTo) : ZergUnit(name, r), morphingToData(morphingTo), morphProgress(0) {

}

void ZergLarva::update() {
	++morphProgress;
	/*
	if (morphProgress == morphingToData.buildTime) {
		morphProgress = morphingToData.buildTime;
	} // not needed with >= in isDone()
	*/
}

bool ZergLarva::isDone() {
	return (morphProgress >= morphingToData.buildTime);
}

EntityData& ZergLarva::getUnitData() {
	return morphingToData;
}


/*
 * Drone
 */
ZergDrone::ZergDrone(string name, ResourceManager& r) : ZergUnit(name, r), working(false) , morphing(false) , morphProgress(0) , morphingToData(nullptr) {
	
}

void ZergDrone::setWorking(bool b) {
	working = b;
}

void ZergDrone::update() {
	if (morphing) {
		++morphProgress;
	}
}

bool ZergDrone::morph(string s) {
	return morph(entityDataMap.at(s));
}

bool ZergDrone::morph(EntityData& e) {
	if (working || morphing)
		return false;
	if (r.canBuild(e)) {
		r.consumeRes(e);
		morphing = true;
		morphProgress = 0;
		morphingToData = &e;
		return true;
	} else {
		return false;
	}
}

bool ZergDrone::isMorphing() {
	return morphing;
}

bool ZergDrone::morphingDone() {
	if (!morphing)
		return false;
	return (morphingToData->buildTime >= morphProgress);
}

EntityData& ZergDrone::getBuildingData() {
	return *morphingToData;
}


/*
 * Queen
 */
ZergQueen::ZergQueen(string name, ResourceManager& r) : ZergUnit(name, r), queenData(entityDataMap.at(string("queen"))), energy(queenData.startEnergy) {
	
}

void ZergQueen::update() {
	energy += FixedPoint(0.5625);
	FixedPoint maxEnergy(queenData.maxEnergy);
	if (energy > maxEnergy) {
		energy = maxEnergy;
	}
}

bool ZergQueen::injectLarva() {
	if (energy >= FixedPoint(25)) {
		energy -= FixedPoint(25);
		return true;
	} else {
		return false;
	}
}

