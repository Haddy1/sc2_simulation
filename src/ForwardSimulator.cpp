#include "../include/ForwardSimulator.h"
#include "../include/EntityData.h"
#include "../include/ZergUnit.h"
#include "../include/ZergBuilding.h"

#include <iostream>

ForwardSimulator::ForwardSimulator(Race r) : race(r), timestep(0), running(true) {
	init();
	
}

ForwardSimulator::ForwardSimulator(Race r, queue<string> q) : race(r), buildOrder(q), timestep(0), running(true) {
	init();
}

void ForwardSimulator::init() {
	//starting conditions
	resourceManager.setMineralWorkers(6);
	switch (race) {
		case TERRAN:
			initTerran();
			break;
		case PROTOSS:
			initProtoss();
			break;
		case ZERG:
			initZerg();
			break;
		default:
			break;
	}
}

void ForwardSimulator::initTerran() {
	
}

void ForwardSimulator::initProtoss() {
	
}

void ForwardSimulator::initZerg() {
	/*
	ZergBuilding hatchery(string("hatchery"));
	buildings.push_back(hatchery);
	ZergUnit overlord(string("overlord"));
	units.push_back(overlord);
	for (int i = 0; i < 6; ++i) {
		//TODO
		ZergUnit drone(string("drone"));
		//drone.setWorking(true);
		workers.push_back(drone);
	}
	*/
}

void ForwardSimulator::simulate() {
	switch (race) {
		case TERRAN:
			simulateTerran();
			break;
		case PROTOSS:
			simulateProtoss();
			break;
		case ZERG:
			simulateZerg();
			break;
		default:
			break;
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
		//for (Building& b : buildings) {
			//b.update();//TODO
		//}
		
		//start/finish abilities
		
		//start build if possible
		if (!buildOrder.empty()) {
			string nextItem = buildOrder.front();
			
			EntityData entityData = entityDataMap.at(nextItem);
			
			if (entityData.isBuilding) {
				std::cout << "nextItem: Building " << nextItem << std::endl;
				ZergBuilding b(nextItem);
				std::cout << "id: " << b.getID() << std::endl;
				buildings.push_back(b);
			} else {
				std::cout << "nextItem: Unit " << nextItem << std::endl;
				ZergUnit u(nextItem);
				std::cout << "id: " << u.getID() << std::endl;
				units.push_back(u);
			}
			
			
			
			buildOrder.pop(); //TODO dont always remove
			
		}
		
		
		//redistribute workers
		
		++timestep;
	}
}
