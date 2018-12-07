#include "../include/ZergBuilding.h"

ZergBuilding::ZergBuilding(string name) : Building(name) {

}

void ZergBuilding::upgrade() {
	//TODO
}

void ZergBuilding::update() {

}


ZergHatchery::ZergHatchery(string name) : ZergBuilding(name) {
	//TODO
}

int ZergHatchery::getLarvaCount() {
	return 0;
	//TODO
}

void ZergHatchery::takeLarva() {
	//TODO
}

void ZergHatchery::spawnQueen() {
	//TODO
}

ZergUnit ZergHatchery::getQueen() {
	return ZergUnit(string());
	//TODO
}

