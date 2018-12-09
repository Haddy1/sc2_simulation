#include "../include/ForwardSimulator.h"
#include "../include/EntityData.h"

#include <iostream>

using std::cout;
using std::endl;

ZergSimulator::ZergSimulator() : timestep(0), maxTime(600), running(true) {
	init();
}

ZergSimulator::ZergSimulator(queue<string> q) : buildOrder(q), timestep(0), maxTime(600), running(true) {
	init();
}

void ZergSimulator::init() {
	cout << "Start Configuration:" << endl;
	
	
	ZergHatchery *hatchery = new ZergHatchery(string("hatchery"), resourceManager);
	cout << "Hatchery id: " << hatchery->getID() << endl;
	hatcheries.push_back(hatchery);
	resourceManager.addSupplyMax(entityDataMap.at(string("hatchery")).supplyProvided);
	
	ZergUnit *overlord = new ZergUnit(string("overlord"), resourceManager);
	cout << "Overlord id: " << overlord->getID() << endl;
	units.push_back(overlord);
	resourceManager.addSupplyMax(entityDataMap.at(string("overlord")).supplyProvided);
	
	for (int i = 0; i < 6; ++i) {
		//TODO
		ZergDrone *drone = new ZergDrone(string("drone"), resourceManager);
		cout << "Drone id: " << drone->getID() << endl;
		//drone.setWorking(true);
		workers.push_back(drone);
	}
	
	resourceManager.setMineralWorkers(6);
	
}

void ZergSimulator::simulate() {
	while (running && (timestep < maxTime)) {//TODO
		
		resourceManager.update();
		cout << resourceManager << endl;
		
		//update/finish buildings
		for (ZergHatchery *b : hatcheries) {
			b->update();//TODO
		}
		for (ZergSpire *b : spires) {
			b->update();//TODO
		}
		for (ZergNydusNetwork *b : nydusNetworks) {
			b->update();//TODO
		}
		//update/finish units
		for (ZergLarva *u : larvas) {
			u->update();//TODO
		}
		for (ZergDrone *u : workers) {
			u->update();//TODO
		}
		for (ZergQueen *u : queens) {
			u->update();//TODO
		}
		
		//start/finish abilities
		
		//start build if possible
		if (!buildOrder.empty()) {
			string nextItem = buildOrder.front();
			
			EntityData entityData = entityDataMap.at(nextItem);
			
			if (entityData.isBuilding) {
				std::cout << "nextItem: Building " << nextItem << std::endl;
				ZergBuilding *b = new ZergBuilding(nextItem, resourceManager);
				std::cout << "id: " << b->getID() << std::endl;
				buildings.push_back(b);
			} else {
				std::cout << "nextItem: Unit " << nextItem << std::endl;
				ZergUnit *u = new ZergUnit(nextItem, resourceManager);
				std::cout << "id: " << u->getID() << std::endl;
				units.push_back(u);
			}
			
			
			
			buildOrder.pop(); //TODO dont always remove
			
		}
		
		
		//redistribute workers
		
		++timestep;
	}
}
