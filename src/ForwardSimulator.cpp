#include "../include/ForwardSimulator.h"

ForwardSimulator::ForwardSimulator(Race r) : race(r), timestep(0), running(true) {
	
}

ForwardSimulator::ForwardSimulator(Race r, queue<string> q) : race(r), buildOrder(q), timestep(0), running(true) {
	
}

void ForwardSimulator::simulate() {
	while (running) {
		resourceManager.update();
		//update/finish buildings
		
		//start/finish abilities
		
		//start build if possible
		
		//redistribute workers
		
		++timestep;
	}
}
