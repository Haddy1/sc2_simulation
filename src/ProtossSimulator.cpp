#include "../include/ForwardSimulator.h"
#include "../include/EntityData.h"

#include <iostream>

ProtossSimulator::ProtossSimulator() : logger(PROTOSS, resourceManager, !buildOrder.empty(), "output/log.txt"), timestep(1), running(true) {
	init();
}

ProtossSimulator::ProtossSimulator(queue<string> q) : buildOrder(q), logger(PROTOSS, resourceManager, !buildOrder.empty(), "output/log.txt"), timestep(1), running(true) {
	init();
}

void ProtossSimulator::init() {
	resourceManager.setMineralWorkers(6);
	//...
}

void ProtossSimulator::simulate() {
	while(timestep < 10) {
		resourceManager.update();
		
		logger.printMessage(timestep, vector<EventEntry>{EventEntry("build-end", "marine"), EventEntry("build-start", "marine")});
		++timestep;
	}
}
