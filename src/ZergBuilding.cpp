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
ZergHatchery::ZergHatchery(string name, ResourceManager& r) : ZergBuilding(name, r) {
	//TODO
}

void ZergHatchery::update() {
	//TODO
	//update upgrade
	if (upgrading) {
		++upgradeProgress;
		if (name == string("hatchery")) {
			if (upgradeProgress == 80) {
				upgrading = false;
				upgradeProgress = 0;
				name = string("lair");
			}
		} else if (name == string("lair")) {
			if (upgradeProgress == 100) {
				upgrading = false;
				upgradeProgress = 0;
				name = string("hive");
			}
		}
		return; //cant do work if upgrading //TODO is this true?
	}
	//update queen spawn
	if (spawningQueen) {
		++queenProgress;
		if (queenProgress > 50) { //TODO read 50 from EntityData queen buildTime
			queenProgress = 50;
		}
	}
	//update regular larva spawn
	if (larvas < 3) {
		++larvaProgress;
		if (larvaProgress == 15) {
			larvas++;
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
	if (name != string("hive") && !upgrading && !spawningQueen && !injectingLarvas) {
		upgrading = true;
		upgradeProgress = 0;
		return true;
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
		spawningQueen = true;
		queenProgress = 0;
		return true;
	} else {
		return false;
	}
}

//bool ZergHatchery::queenDone() {
//	return queenProgress == 50;
//}

bool ZergHatchery::takeQueen() {
	if (queenProgress == 50) {
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
ZergSpire::ZergSpire(string name, ResourceManager& r) : ZergBuilding(name, r), upgrading(false), upgradeProgress(0) {
	
}

bool ZergSpire::upgrade() {
	if (name == string("spire") && !upgrading) {
		upgrading = true;
		upgradeProgress = 0;
		return true;
	} else {
		return false;
	}
}

void ZergSpire::update() {
	if (upgrading) {
		++upgradeProgress;
		if (upgradeProgress == 100) {
			name = string("greater_spire");
			upgrading = false;
			upgradeProgress = 0;
		}
	}
}


/*
 * Nydus Network
 */
ZergNydusNetwork::ZergNydusNetwork(string name, ResourceManager& r) : ZergBuilding(name, r), spawningUnit(false), spawnProgress(0) {
	
}

void ZergNydusNetwork::update() {
	if (spawningUnit) {
		++spawnProgress;
		if (spawnProgress > 20) {
			spawnProgress = 20;
		}
	}
}

bool ZergNydusNetwork::spawn() {
	if (!spawningUnit) {
		spawningUnit = true;
		spawnProgress = 0;
		return true;
	} else {
		return false;
	}
}

bool ZergNydusNetwork::takeUnit() {//nydus worm
	if (spawnProgress == 20) {
		spawningUnit = false;
		spawnProgress = 0;
		return true;
	} else {
		return false;
	}
}


