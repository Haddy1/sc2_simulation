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
		vec.push_back(create_event_ptr("build-start", "marine", 123, 456));
		vec.push_back(create_event_ptr("build-end", "marine", 111, 222));
		vec.push_back(create_ability_ptr("special", "chronoboost", "some_nexus", "some_building"));
		
		logger.printMessage(timestep, vec);
		++timestep;
	}
}
