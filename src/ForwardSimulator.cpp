#include "../include/ForwardSimulator.h"
#include "../include/UnitData.h"
#include "../include/BuildingData.h"

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
			
			if (unitDataMap.find(nextItem) != unitDataMap.end()) {
				//next item is a unit
				std::cout << "next unit: " << nextItem << std::endl;
			} else if (buildingDataMap.find(nextItem) != buildingDataMap.end()) {
				//next item is a building
				std::cout << "next building: " << nextItem << std::endl;
			} else {
				//error: neither unit or building
				std::cerr << "error: item in build order does not exist" << std::endl;
			}
		}
		
		
		//redistribute workers
		
		++timestep;
	}
}
