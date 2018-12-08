#include "../include/ForwardSimulator.h"
#include "../include/EntityData.h"

#include <iostream>

ProtossSimulator::ProtossSimulator() : timestep(0), running(true) {
	init();
}

ProtossSimulator::ProtossSimulator(queue<string> q) : buildOrder(q), timestep(0), running(true) {
	init();
}

void ProtossSimulator::init() {
	resourceManager.setMineralWorkers(6);
	//...
}

void ProtossSimulator::simulate() {
	
}
