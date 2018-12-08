#include "../include/ForwardSimulator.h"
#include "../include/EntityData.h"

#include <iostream>

TerranSimulator::TerranSimulator() : timestep(0), running(true) {
	init();
}

TerranSimulator::TerranSimulator(queue<string> q) : buildOrder(q), timestep(0), running(true) {
	init();
}

void TerranSimulator::init() {
	resourceManager.setMineralWorkers(6);
	//...
}

void TerranSimulator::simulate() {
	
}
