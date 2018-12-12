#include "../include/ZergUnit.h"

#include <iostream>

using std::cout;
using std::endl;

/*
 * Generic Unit that cant morph
 */
ZergUnit::ZergUnit(string name, ResourceManager& r) : Unit(name), r(r) {
	r.addSupplyMax(entityDataMap.at(name).supplyProvided);
	cout << "Unit " << name << " with id=" << getID() << " created." << endl;
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
	if (morphing)
		return false;
	if (!dependencyFulfilled(e)) {
		return false;
	}
	if (r.canBuild(e)) {
		r.consumeRes(e);
		morphing = true;
		morphProgress = 0;
		morphingToData = &e;
		working = false;
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
	return (morphingToData->buildTime <= morphProgress);
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

bool ZergQueen::canInjectLarvas() {
	return (energy >= FixedPoint(25));
}

bool ZergQueen::injectLarvas() {
	if (energy >= FixedPoint(25)) {
		energy -= FixedPoint(25);
		return true;
	} else {
		return false;
	}
}


/*
 * Upgradeable Unit: Overlord, Zergling, Corruptor
 */
/*
class ZergUpgradeableUnit : public ZergUnit {
	EntityData& unitData;
	bool upgrading;
	bool upgradeProgress;
public:
	ZergUpgradeableUnit(string, ResourceManager&);
	void update();
	bool upgrade();
};
*/
ZergUpgradeableUnit::ZergUpgradeableUnit(string name, ResourceManager& r) : ZergUnit(name, r), unitData(entityDataMap.at(name)), upgradeData(nullptr), upgrading(false), upgradeProgress(0) {
	
}

void ZergUpgradeableUnit::update() {
	if (upgrading) {
		++upgradeProgress;
		if (upgradeProgress == upgradeData->buildTime) {//TODO
			upgrading = false;
			name = upgradeData->name;
			unitData = *upgradeData;
		}
	}
}

bool ZergUpgradeableUnit::upgrade() {
	if (upgrading)
		return false;
	
	if (name == string("overlord")) {
		upgradeData = &entityDataMap.at(string("overseer"));
	} else if (name == string("zergling")) {
		upgradeData = &entityDataMap.at(string("baneling"));
	} else if (name == string("corruptor")) {
		upgradeData = &entityDataMap.at(string("brood_lord"));
	} else {
		return false;
	}
	
	if (!dependencyFulfilled(*upgradeData)) {
		return false;
	}
	if (r.canBuild(*upgradeData)) {
		r.consumeRes(*upgradeData);
		upgrading = true;
		upgradeProgress = 0;
		return true;
	} else {
		return false;
	}
}






