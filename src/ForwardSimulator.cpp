#include "../include/ForwardSimulator.h"
#include "../include/EntityData.h"

#include <iostream>

ForwardSimulator::ForwardSimulator(Race r) : race(r), timestep(0), running(true) {
	resourceManager.setMineralWorkers(6);
}

ForwardSimulator::ForwardSimulator(Race r, queue<string> q) : race(r), buildOrder(q), timestep(0), running(true) {
	resourceManager.setMineralWorkers(6);
}

void ForwardSimulator::simulate() {
	switch (race) {
		case TERRAN:
			simulateTerran(); break;
		case PROTOSS:
			simulateProtoss(); break;
		case ZERG:
			simulateZerg(); break;
		default: break;
	}
}

void ForwardSimulator::simulateTerran() {
	
}

void ForwardSimulator::simulateProtoss() {
	
}

void ForwardSimulator::simulateZerg() {
	while (timestep < 20) {//TODO
		
		resourceManager.update();
		
		std::cout << resourceManager << std::endl;
		
		//update/finish buildings
		for (Building& b : buildings) {
			b.update();
		}
		
		//start/finish abilities
		
		//start build if possible
		if (!buildOrder.empty()) {
			string nextItem = buildOrder.front();
			
			EntityData entityData = entityDataMap.at(nextItem);
			
			if (entityData.isBuilding) {
				std::cout << "nextItem: Building " << nextItem << std::endl;
				Building b(nextItem);
				std::cout << "id: " << b.getID() << std::endl;
				buildings.push_back(b);
			} else {
				std::cout << "nextItem: Unit " << nextItem << std::endl;
				Unit u(nextItem);
				std::cout << "id: " << u.getID() << std::endl;
				units.push_back(u);
			}
			
			
			
			buildOrder.pop(); //TODO dont always remove
			
		}
		
		
		//redistribute workers
		
		++timestep;
	}
}
