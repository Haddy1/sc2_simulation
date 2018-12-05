#include "../include/Building.h"

Building::Building(string name) : constructing(true), constrProgress(0), buildingType(name), energy(0), working(false), workProgress(0) {
	
}

Building::~Building() {
	
}

void Building::update() {
	//todo
}

bool Building::busy() {
	return constructing || working;
}

bool Building::ability() {
	//todo
	return false;
}

bool Building::createUnit(string) {
	//todo
	return false;
}

