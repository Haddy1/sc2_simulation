#include "../include/ZergBuilding.h"

/*
 * Generic Zerg Building (tech tree only)
 */
ZergBuilding::ZergBuilding(string name) : Building(name) {

}

void ZergBuilding::update() {
	
}


/*
 * Hatchery, Lair, Hive
 */
ZergHatchery::ZergHatchery(string name) : ZergBuilding(name) {
	//TODO
}

void ZergHatchery::update() {
	//TODO
	if (spawningQueen) {
		++queenProgress;
		if (queenProgress > 50) { //TODO read 50 from EntityData queen buildTime
			queenProgress = 50;
		}
	}
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
	}
	if (larvas < 3) {
		++larvaProgress;
		if (larvaProgress == 15) {
			larvas++;
			larvaProgress = 0;
		}
	}
	if (injectingLarvas) {
		//TODO
	}
}

void ZergHatchery::upgrade() {
	if (name != string("hive") && !upgrading) {
		upgrading = true;
		upgradeProgress = 0;
	}
}

int ZergHatchery::getLarvaCount() const {
	return larvas;
}

void ZergHatchery::takeLarva() {
	//TODO
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

bool ZergHatchery::queenDone() {
	return queenProgress == 50;
}

ZergQueen* ZergHatchery::getQueen() {
	queenProgress = 0;
	spawningQueen = false;
	ZergQueen *queen = new ZergQueen(string("queen"));
	return queen;
}

void ZergHatchery::injectLarvas() {
	//TODO 4 eggs, 40sec, max 19
}


/*
 * Spire, Greater Spire
 */
ZergSpire::ZergSpire(string name) : ZergBuilding(name) {
	//TODO
}

void ZergSpire::upgrade() {
	//TODO
}

void ZergSpire::update() {
	//TODO
}


/*
 * Nydus Network
 */
ZergNydusNetwork::ZergNydusNetwork(string name) : ZergBuilding(name) {
	//TODO
}

void ZergNydusNetwork::update() {
	//TODO
}

ZergUnit* ZergNydusNetwork::getUnit() {
	//TODO
}


