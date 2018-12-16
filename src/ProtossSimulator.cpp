#include "../include/ForwardSimulator.h"
#include "../include/EntityData.h"

#include <iostream>

ProtossSimulator::ProtossSimulator(bool validBuildlist) : logger(PROTOSS, resourceManager, validBuildlist, "output/log.txt"), timestep(1), running(true) {
	
}

ProtossSimulator::ProtossSimulator(queue<string> q, bool validBuildlist) : buildOrder(q), logger(PROTOSS, resourceManager, validBuildlist, "output/log.txt"), timestep(1), running(true) {
	
}

void ProtossSimulator::init() {
	resourceManager.setMineralWorkers(6);
	// test
	vector<pair<string, vector<int>>> initUnits = {
		make_pair<string, vector<int>>("scv", {0, 1, 2, 3, 4, 5}),
		make_pair<string, vector<int>>("command_center", {6})
	};
	logger.printSetup(initUnits);
	logger.printMessage(0);
	//...
}

void ProtossSimulator::simulate() {
	while(timestep < 10) {
		resourceManager.update();
		
		vector<EventEntry*> vec;
		EventEntry a("build-start", "marine");
		EventEntry b("build-end", "marine");
		AbilityEntry c("special", "chronoboost");
		vec.push_back(&a); vec.push_back(&b); vec.push_back(&c);
		
		logger.printMessage(timestep, vec);
		++timestep;
	}
}
