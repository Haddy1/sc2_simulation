#include "../include/ForwardSimulator.h"
#include "../include/EntityData.h"

#include <memory>

using std::shared_ptr;
using std::make_shared;

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
	logger.printMessageStart();
	//...
}

void ProtossSimulator::simulate() {
	while(timestep < 10) {
		resourceManager.update();
		
		vector<shared_ptr<EventEntry>> vec;
		/*
		EventEntry a("build-start", "marine");
		EventEntry b("build-end", "marine");
		AbilityEntry c("special", "chronoboost", "some_nexus", "some_building");
		vec.push_back(&a); vec.push_back(&b); vec.push_back(&c);
		*/
		vec.push_back(make_shared<EventEntry>(EventEntry("build-start", "marine", 1, 2)));
		vec.push_back(make_shared<EventEntry>(EventEntry("build-end", "marine", 1, 2)));
		vec.push_back(make_shared<AbilityEntry>(AbilityEntry("special", "chronoboost", "some_nexus", "some_building")));
		
		logger.printMessage(timestep, vec);
		++timestep;
	}
}
