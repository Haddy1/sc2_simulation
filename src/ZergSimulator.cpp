#include "../include/ForwardSimulator.h"
#include "../include/EntityData.h"

#include <iostream>

ZergSimulator::ZergSimulator() : timestep(0), running(true) {
	init();
}

ZergSimulator::ZergSimulator(queue<string> q) : buildOrder(q), timestep(0), running(true) {
	init();
}

void ZergSimulator::init() {
	resourceManager.setMineralWorkers(6);
	
	ZergBuilding *hatchery = new ZergBuilding(string("hatchery"));
	std::cout << "Hatchery id: " << hatchery->getID() << std::endl;
	buildings.push_back(hatchery);
	ZergUnit *overlord = new ZergUnit(string("overlord"));
	std::cout << "Overlord id: " << overlord->getID() << std::endl;
	units.push_back(overlord);
	for (int i = 0; i < 6; ++i) {
		//TODO
		ZergUnit *drone = new ZergUnit(string("drone"));
		std::cout << "Drone id: " << drone->getID() << std::endl;
		//drone.setWorking(true);
		workers.push_back(drone);
	}
	
}

void ZergSimulator::simulate() {
	while (timestep < 20) {//TODO
		
		resourceManager.update();
		std::cout << resourceManager << std::endl;
		
		//update/finish buildings
		for (Building *b : buildings) {
			b->update();//TODO
		}
		
		//start/finish abilities
		
		//start build if possible
		if (!buildOrder.empty()) {
			string nextItem = buildOrder.front();
			
			EntityData entityData = entityDataMap.at(nextItem);
			
			if (entityData.isBuilding) {
				std::cout << "nextItem: Building " << nextItem << std::endl;
				ZergBuilding *b = new ZergBuilding(nextItem);
				std::cout << "id: " << b->getID() << std::endl;
				buildings.push_back(b);
			} else {
				std::cout << "nextItem: Unit " << nextItem << std::endl;
				ZergUnit *u = new ZergUnit(nextItem);
				std::cout << "id: " << u->getID() << std::endl;
				units.push_back(u);
			}
			
			
			
			buildOrder.pop(); //TODO dont always remove
			
		}
		
		
		//redistribute workers
		
		++timestep;
	}
}
