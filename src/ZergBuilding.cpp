#include "../include/ZergBuilding.h"

#include <iostream>

using std::cout;
using std::endl;

/*
 * Generic Zerg Building (tech tree only)
 */
ZergBuilding::ZergBuilding(int& ID_Counter, EntityType type, ResourceManager& r, Tech& tech) : Building(ID_Counter, type), r(r), tech(tech) {
	r.addSupplyMax(entityData->supplyProvided);
	tech.add(type);
}


/*
 * Hatchery, Lair, Hive
 */
ZergHatchery::ZergHatchery(int& ID_Counter, EntityType type, ResourceManager& r, Tech& tech, int& busyCounter) :
	ZergBuilding(ID_Counter, type, r, tech), lairData(entityDataMap.at(lair)), hiveData(entityDataMap.at(hive)), queenData(entityDataMap.at(queen)), larvas(3), spawningLarva(false), larvaProgress(0), spawningQueen(false), queenProgress(0) , injectingLarvas(false) , injectProgress(0) , upgrading(false) , upgradeProgress(0), busyCounter(busyCounter) 
{
	
}

bool ZergHatchery::update() {
	bool justUpgraded = false;
	//update upgrade
	if (upgrading) {
		++upgradeProgress;
		if (entityData->type == hatchery) {
			if (upgradeProgress == lairData.buildTime) {
				upgrading = false;
				upgradeProgress = 0;
				entityData = &lairData;
				tech.add(lair);
				tech.remove(hatchery);
				justUpgraded = true;
				--busyCounter;
			}
		} else if (entityData->type == lair) {
			if (upgradeProgress == hiveData.buildTime) {
				upgrading = false;
				upgradeProgress = 0;
				entityData = &hiveData;
				tech.add(hive);
				tech.remove(lair);
				justUpgraded = true;
				--busyCounter;
			}
		}
	}
	
	//update queen spawn
	if (spawningQueen) {
		++queenProgress;
		if (queenProgress > queenData.buildTime) {
			queenProgress = queenData.buildTime;
		}
	}
	
	//update regular larva spawn
	if (spawningLarva) {
		++larvaProgress;
		if (larvaProgress == 15) {
			++larvas;
			larvaProgress = 0;
			if (larvas >= 3) {
				spawningLarva = false;
			}
		}
	}
	
	//update special ability
	if (injectingLarvas) {
		++injectProgress;
		if (injectProgress == 40) {
			larvas += 4;
			if (larvas > 19) {
				larvas = 19;
			}
			injectingLarvas = false;
			injectProgress = 0;
			spawningLarva = false; // there is more than 3 larvas, so pause natural spawning
		}
	}
	
	return justUpgraded;
}

bool ZergHatchery::upgrade() {
	if (upgrading || spawningQueen) {
		return false;
	}
	if (entityData->type == hatchery) {
		if (!tech.dependencyFulfilled(lairData)) {
			return false;
		}
		if (r.consumeRes(lairData)) {
			upgrading = true;
			upgradeProgress = 0;
			++busyCounter;
			return true;
		}
	} else if (entityData->type == lair) {
		if (!tech.dependencyFulfilled(hiveData)) {
			return false;
		}
		if (r.consumeRes(hiveData)) {
			upgrading = true;
			upgradeProgress = 0;
			++busyCounter;
			return true;
		}
	}
	return false;
	//cant upgrade if doing work //TODO verify this
}

int ZergHatchery::getLarvaCount() const {
	return larvas;
}

bool ZergHatchery::morphLarva(EntityType s) {
	EntityData& entity = entityDataMap.at(s);
	return morphLarva(entity);
}

bool ZergHatchery::morphLarva(EntityData& entity) {
	if (larvas == 0)
		return false;
	if (!tech.dependencyFulfilled(entity)) {
		return false;
	}
	int num = (entity.type == zergling ? 2 : 1);
	if (r.consumeRes(entity, num)) {
		--larvas;
		if (!spawningLarva && larvas < 3) {
			spawningLarva = true;
			larvaProgress = 0;
		}
		return true;
	}
	return false;
}

bool ZergHatchery::spawnQueen() {
	if (upgrading || spawningQueen) {
		return false;
	}
	if (!tech.dependencyFulfilled(queenData)) {
		return false;
	}
	if (r.consumeRes(queenData)) {
		spawningQueen = true;
		queenProgress = 0;
		++busyCounter;
		return true;
	}
	return false;
}

bool ZergHatchery::takeQueen() {
	if (queenProgress >= queenData.buildTime) {
		queenProgress = 0;
		spawningQueen = false;
		--busyCounter;
		return true;
	} else {
		return false;
	}
}

bool ZergHatchery::injectLarvas() {
	if ((!injectingLarvas) && larvas < 19) {
		injectingLarvas = true;
		injectProgress = 0;
		return true;
	} else {
		return false;
	}
}

bool ZergHatchery::busy() {
	return (upgrading || spawningQueen);
}


/*
 * Spire, Greater Spire
 */
ZergSpire::ZergSpire(int& ID_Counter, EntityType type, ResourceManager& r, Tech& tech, int& busyCounter) : ZergBuilding(ID_Counter, type, r, tech), greaterSpireData(entityDataMap.at(greater_spire)), upgrading(false), upgradeProgress(0), busyCounter(busyCounter) {
	
}

bool ZergSpire::upgrade() {
	if (upgrading) {
		return false;
	}
	if (entityData->type != spire) {
		return false;
	}
	if (!tech.dependencyFulfilled(greaterSpireData)) {
		return false;
	}
	if (r.consumeRes(greaterSpireData)) {
		upgrading = true;
		upgradeProgress = 0;
		++busyCounter;
		return true;
	}
	return false;
}

bool ZergSpire::update() {
	if (upgrading) {
		++upgradeProgress;
		if (upgradeProgress == greaterSpireData.buildTime) {
			entityData = &greaterSpireData;
			upgrading = false;
			upgradeProgress = 0;
			tech.add(greater_spire);
			tech.remove(spire);
			--busyCounter;
			return true;
		}
	}
	return false;
}

bool ZergSpire::busy() {
	return (upgrading);
}


/*
 * Nydus Network
 */
ZergNydusNetwork::ZergNydusNetwork(int& ID_Counter, EntityType type, ResourceManager& r, Tech& tech, int& busyCounter) : ZergBuilding(ID_Counter, type, r, tech), nydusWormData(entityDataMap.at(nydus_worm)), spawningUnit(false), spawnProgress(0), busyCounter(busyCounter) {
	
}

void ZergNydusNetwork::update() {
	if (spawningUnit) {
		++spawnProgress;
	}
}

bool ZergNydusNetwork::spawn() {
	if (spawningUnit) {
		return false;
	}
	if (!tech.dependencyFulfilled(nydusWormData)) {
		return false;
	}
	if (r.consumeRes(nydusWormData)) {
		spawningUnit = true;
		spawnProgress = 0;
		++busyCounter;
		return true;
	}
	return false;
}

bool ZergNydusNetwork::takeUnit() {//nydus worm
	if (spawnProgress >= nydusWormData.buildTime) {
		spawningUnit = false;
		spawnProgress = 0;
		--busyCounter;
		return true;
	} else {
		return false;
	}
}

bool ZergNydusNetwork::busy() {
	return (spawningUnit);
}


