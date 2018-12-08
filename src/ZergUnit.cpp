#include "../include/ZergUnit.h"

/*
 * Generic Unit that cant morph
 */
ZergUnit::ZergUnit(string name) : Unit(name) {

}


/*
 * Larva
 */
ZergLarva::ZergLarva(string name) : ZergUnit(name) {

}


/*
 * Drone
 */
ZergDrone::ZergDrone(string name) : ZergUnit(name) {

}

void ZergDrone::setWorking(bool b) {
	working = b;
}

bool ZergDrone::morph(string s) {
	//TODO
}

//ZergBuilding* ZergDrone::getMorphedBuilding() {
	//TODO
//}


/*
 * Queen
 */
ZergQueen::ZergQueen(string name) : ZergUnit(name) {
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

