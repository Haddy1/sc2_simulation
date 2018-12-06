#include "../include/ForwardSimulator.h"
#include "../include/EntityData.h"

#include <iostream>

ForwardSimulator::ForwardSimulator(Race r) : race(r), timestep(0), running(true) {
	
}

ForwardSimulator::ForwardSimulator(Race r, queue<string> q) : race(r), buildOrder(q), timestep(0), running(true) {
	
}

void ForwardSimulator::simulate() {
	while (running) {
		
		resourceManager.update();
		
		//update/finish buildings
		for (Building& b : buildings) {
			b.update();
		}
		
		//start/finish abilities
		
		//start build if possible
		if (!buildOrder.empty()) {
			string nextItem = buildOrder.front();
			
			auto it = entityDataMap.find(nextItem);
			EntityData entityData = it->second;
			if (entityData.isBuilding) {
				std::cout << "nextItem: Building " << nextItem << std::endl;
			} else {
				std::cout << "nextItem: Unit " << nextItem << std::endl;
			}

			buildOrder.pop(); //TODO dont always remove
			
		}
		
		
		//redistribute workers
		
		++timestep;
	}
}
