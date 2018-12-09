#include "../include/ZergUnit.h"

/*
 * Generic Unit that cant morph
 */
ZergUnit::ZergUnit(string name, ResourceManager& r) : Unit(name), r(r) {

}


/*
 * Larva
 */
ZergLarva::ZergLarva(string name, ResourceManager& r) : ZergUnit(name, r) {

}


/*
 * Drone
 */
ZergDrone::ZergDrone(string name, ResourceManager& r) : ZergUnit(name, r) {

}

void ZergDrone::setWorking(bool b) {
	working = b;
}

bool ZergDrone::morph(string s) {
	//TODO
	return false;
}

//ZergBuilding* ZergDrone::getMorphedBuilding() {
	//TODO
//}


/*
 * Queen
 */
ZergQueen::ZergQueen(string name, ResourceManager& r) : ZergUnit(name, r) {
	//TODO
}

void ZergQueen::update() {
	//TODO
}

bool ZergQueen::injectLarva() {
	if (energy >= 25) {
		energy -= 25;
		return true;
	} else {
		return false;
	}
}

