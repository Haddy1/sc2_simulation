#include "../include/ZergUnit.h"

#include <iostream>

using std::cout;
using std::endl;

/*
 * Generic Unit that cant morph
 */
ZergUnit::ZergUnit(int& ID_Counter, EntityType type, ResourceManager& r) : Unit(ID_Counter, type), r(r) {
	r.addSupplyMax(entityData->supplyProvided);
}


/*
 * Larva
 */
ZergLarva::ZergLarva(int& ID_Counter, EntityType type, ResourceManager& r, EntityType morphingTo, int& busyCounter) : ZergUnit(ID_Counter, type, r), morphingToData(&entityDataMap.at(morphingTo)), morphProgress(0), busyCounter(busyCounter) {
	//++busyCounter;
}

ZergLarva::ZergLarva(int& ID_Counter, EntityType type, ResourceManager& r, EntityData *morphingTo, int& busyCounter) : ZergUnit(ID_Counter, type, r), morphingToData(morphingTo), morphProgress(0), busyCounter(busyCounter) {
	//++busyCounter;
}

//ZergLarva::~ZergLarva() {
//	--busyCounter;
//}

void ZergLarva::update() {
	++morphProgress;
}

bool ZergLarva::isDone() {
	return (morphProgress >= morphingToData->buildTime);
}

EntityData *ZergLarva::getUnitData() {
	return morphingToData;
}

bool ZergLarva::busy() {
	return true;
}


/*
 * Drone
 */
ZergDrone::ZergDrone(int& ID_Counter, EntityType type, ResourceManager& r, Tech& tech, int& busyCounter) : ZergUnit(ID_Counter, type, r), morphing(false) , morphProgress(0) , morphingToData(nullptr), busyCounter(busyCounter), tech(tech) {
	
}

//ZergDrone::~ZergDrone() {
//	--busyCounter;
//}

void ZergDrone::update() {
	if (morphing) {
		++morphProgress;
	}
}

bool ZergDrone::morph(EntityType s) {
	return morph(&entityDataMap.at(s));
}

bool ZergDrone::morph(EntityData *e) {
	if (morphing) {
		return false;
	}
	if (!tech.dependencyFulfilled(*e)) {
		return false;
	}
	if (r.consumeRes(*e)) {
		r.decrementSupply();
		morphing = true;
		morphProgress = 0;
		morphingToData = e;
		//++busyCounter;
		return true;
	} else {
		return false;
	}
}

bool ZergDrone::isMorphing() {
	return morphing;
}

bool ZergDrone::morphingDone() {
	if (!morphing) {
		return false;
	}
	return (morphProgress >= morphingToData->buildTime);
}

EntityData *ZergDrone::getBuildingData() {
	return morphingToData;
}

bool ZergDrone::busy() {
	return morphing;
}


/*
 * Queen
 */
FixedPoint ZergQueen::chargeRate(0.5625);
FixedPoint ZergQueen::fixed25(25);

ZergQueen::ZergQueen(int& ID_Counter, EntityType type, ResourceManager& r, int& busyCounter) : ZergUnit(ID_Counter, type, r), energy(entityData->startEnergy), maxEnergy(entityData->maxEnergy), busyCounter(busyCounter) {
	
}

void ZergQueen::update() {
	energy += chargeRate;
	//FixedPoint maxEnergy(entityData->maxEnergy);
	if (energy > maxEnergy) {
		energy = maxEnergy;
	}
}

bool ZergQueen::canInjectLarvas() {
	return (energy >= fixed25);
}

bool ZergQueen::injectLarvas() {
	if (energy >= fixed25) {
		energy -= fixed25;
		return true;
	} else {
		return false;
	}
}

bool ZergQueen::busy() {
	return false; //TODO injecting larvas??
}


/*
 * Upgradeable Unit: Overlord, Zergling, Corruptor
 */
ZergUpgradeableUnit::ZergUpgradeableUnit(int& ID_Counter, EntityType type, ResourceManager& r, Tech& tech, int& busyCounter) : ZergUnit(ID_Counter, type, r), upgradeData(nullptr), upgrading(false), upgradeProgress(0), busyCounter(busyCounter), tech(tech) {
	
}

bool ZergUpgradeableUnit::update() {
	if (upgrading) {
		++upgradeProgress;
		if (upgradeProgress == upgradeData->buildTime) {
			upgrading = false;
			entityData = upgradeData;
			--busyCounter;
			return true;
		}
	}
	return false;
}

bool ZergUpgradeableUnit::upgrade() {
	if (upgrading) {
		return false;
	}
	
	if (entityData->type == overlord) {
		upgradeData = &entityDataMap.at(overseer);
	} else if (entityData->type == zergling) {
		upgradeData = &entityDataMap.at(baneling);
	} else if (entityData->type == corruptor) {
		upgradeData = &entityDataMap.at(brood_lord);
	} else {
		return false;
	}
	
	if (!tech.dependencyFulfilled(*upgradeData)) {
		return false;
	}
	
	r.decreaseSupply(entityData->supplyCost); // since we are morphing, take away the supply this unit currently uses
	if (r.consumeRes(*upgradeData)) {
		upgrading = true;
		upgradeProgress = 0;
		++busyCounter;
		return true;
	} else {
		r.increaseSupply(entityData->supplyCost); // failed to build, put supply back
		return false;
	}
}

bool ZergUpgradeableUnit::busy() {
	return upgrading;
}






