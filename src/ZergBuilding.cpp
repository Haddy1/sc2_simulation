#include "../include/ZergBuilding.h"

/*
 * Generic Zerg Building (tech tree only)
 */
ZergBuilding::ZergBuilding(string name, ResourceManager& r) : Building(name), r(r) {

}

void ZergBuilding::update() {
	//nothing
}


/*
 * Hatchery, Lair, Hive
 */
ZergHatchery::ZergHatchery(string name, ResourceManager& r) :
	ZergBuilding(name, r), lairData(entityDataMap.at(string("lair"))), hiveData(entityDataMap.at(string("hive"))), queenData(entityDataMap.at(string("queen"))), larvas(0), larvaProgress(0), spawningQueen(false), queenProgress(0) , injectingLarvas(false) , injectProgress(0) , upgrading(false) , upgradeProgress(0) 
{
	//TODO
}

void ZergHatchery::update() {
	//TODO
	//update upgrade
	if (upgrading) {
		++upgradeProgress;
		if (name == string("hatchery")) {
			if (upgradeProgress == lairData.buildTime) {
				upgrading = false;
				upgradeProgress = 0;
				name = string("lair");
			}
		} else if (name == string("lair")) {
			if (upgradeProgress == hiveData.buildTime) {
				upgrading = false;
				upgradeProgress = 0;
				name = string("hive");
			}
		}
		//return; //cant do work if upgrading //TODO is this true?
	}
	//update queen spawn
	if (spawningQueen) {
		++queenProgress;
		if (queenProgress > queenData.buildTime) { //TODO read 50 from EntityData queen buildTime
			queenProgress = queenData.buildTime;
		}
	}
	//update regular larva spawn
	if (larvas < 3) {
		++larvaProgress;
		if (larvaProgress == 15) {
			++larvas;
			larvaProgress = 0;
		}
	}
	//update special ability
	if (injectingLarvas) {
		//TODO
		++injectProgress;
		if (injectProgress == 40) {
			larvas += 4;
			if (larvas > 19) {
				larvas = 19;
			}
			injectingLarvas = false;
			injectProgress = 0;
			larvaProgress = 0; //when inject is done, there is more than 3 larvas so natural spawning resets //TODO ?
		}
	}
}

bool ZergHatchery::upgrade() {
	//cant upgrade if doing work
	if (!upgrading && !spawningQueen && !injectingLarvas) {
		if (name == string("hatchery")) {
			if (r.canBuild(lairData)) { //TODO replace by if (r.consumeRes(...)) correct ?
				r.consumeRes(lairData);
				upgrading = true;
				upgradeProgress = 0;
				return true;
			} else {
				return false;
			}
		} else if (name == string("lair")) {
			if (r.canBuild(lairData)) {
				r.consumeRes(lairData);
				upgrading = true;
				upgradeProgress = 0;
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	} else {
		return false;
	}
}

int ZergHatchery::getLarvaCount() const {
	return larvas;
}

bool ZergHatchery::takeLarva() {
	if (larvas > 0) {
		--larvas;
		return true;
	} else {
		return false;
	}
}

bool ZergHatchery::spawnQueen() {
	if (!spawningQueen) {
		if (r.canBuild(queenData)) {
			r.consumeRes(queenData);
			spawningQueen = true;
			queenProgress = 0;
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

//bool ZergHatchery::queenDone() {
//	return queenProgress == 50;
//}

bool ZergHatchery::takeQueen() {
	if (queenProgress == queenData.buildTime) {
		queenProgress = 0;
		spawningQueen = false;
		return true;
	} else {
		return false;
	}
}

bool ZergHatchery::injectLarvas() {
	//TODO 4 eggs, 40sec, max 19
	if (!injectingLarvas) {
		injectingLarvas = true;
		injectProgress = 0;
		return true;
	} else {
		return false;
	}
}


/*
 * Spire, Greater Spire
 */
ZergSpire::ZergSpire(string name, ResourceManager& r) : ZergBuilding(name, r), greaterSpireData(entityDataMap.at(string("greater_spire"))), upgrading(false), upgradeProgress(0) {
	
}

bool ZergSpire::upgrade() {
	if (name == string("spire") && !upgrading) {
		if (r.canBuild(greaterSpireData)) {
			r.consumeRes(greaterSpireData);
			upgrading = true;
			upgradeProgress = 0;
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

void ZergSpire::update() {
	if (upgrading) {
		++upgradeProgress;
		if (upgradeProgress == greaterSpireData.buildTime) {
			name = string("greater_spire");
			upgrading = false;
			upgradeProgress = 0;
		}
	}
}


/*
 * Nydus Network
 */
ZergNydusNetwork::ZergNydusNetwork(string name, ResourceManager& r) : ZergBuilding(name, r), nydusWormData(entityDataMap.at(string("nydus_worm"))), spawningUnit(false), spawnProgress(0) {
	
}

void ZergNydusNetwork::update() {
	if (spawningUnit) {
		++spawnProgress;
		if (spawnProgress > nydusWormData.buildTime) {
			spawnProgress = nydusWormData.buildTime;
		}
	}
}

bool ZergNydusNetwork::spawn() {
	if (!spawningUnit) {
		if (r.canBuild(nydusWormData)) {
			r.consumeRes(nydusWormData);
			spawningUnit = true;
			spawnProgress = 0;
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool ZergNydusNetwork::takeUnit() {//nydus worm
	if (spawnProgress == nydusWormData.buildTime) {
		spawningUnit = false;
		spawnProgress = 0;
		return true;
	} else {
		return false;
	}
}


