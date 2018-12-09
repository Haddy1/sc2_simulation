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
	
	ZergUpgradeableUnit *overlord = new ZergUpgradeableUnit(string("overlord"), resourceManager);
	cout << "Overlord id: " << overlord->getID() << endl;
	upgradeableUnits.push_back(overlord);
	resourceManager.addSupplyMax(entityDataMap.at(string("overlord")).supplyProvided);
	
	for (int i = 0; i < 6; ++i) {
		ZergDrone *drone = new ZergDrone(string("drone"), resourceManager);
		cout << "Drone id: " << drone->getID() << endl;
		//drone.setWorking(true);
		drones.push_back(drone);
		resourceManager.addSupplyMax(entityDataMap.at(string("drone")).supplyProvided);
	}
	
	resourceManager.setMineralWorkers(6);
	
}

void ZergSimulator::simulate() {
	while (running && (timestep < maxTime)) {//TODO
		
		resourceManager.update();
		cout << resourceManager << endl;
		
		//update/finish buildings
		for (ZergHatchery *b : hatcheries) {
			b->update();
		}
		for (ZergSpire *b : spires) {
			b->update();
		}
		for (ZergNydusNetwork *b : nydusNetworks) {
			b->update();
			if (b->takeUnit()) {
				ZergUnit *nydusWorm = new ZergUnit(string("nydus_worm"), resourceManager);
				cout << "Nydus Worm id: " << nydusWorm->getID() << endl;
				units.push_back(nydusWorm);
				resourceManager.addSupplyMax(entityDataMap.at(string("nydus_worm")).supplyProvided);
			}
		}
		
		//update/finish units
		auto it = larvas.begin();
		while (it != larvas.end()) {
			ZergLarva *u = (*it);
			u->update();
			if (u->isDone()) {
				EntityData& entityData = u->getUnitData();
				//create new unit
				if (entityData.name == string("drone")) {
					ZergDrone *drone = new ZergDrone(string("drone"), resourceManager);
					drones.push_back(drone);
					resourceManager.addSupplyMax(entityDataMap.at(string("drone")).supplyProvided);
				} else if ((entityData.name == string("overlord")) || (entityData.name == string("zergling")) || (entityData.name == string("corruptor"))) {
					ZergUpgradeableUnit *unit = new ZergUpgradeableUnit(entityData.name, resourceManager);
					upgradeableUnits.push_back(unit);
					resourceManager.addSupplyMax(entityDataMap.at(entityData.name).supplyProvided);
				} else {
					ZergUnit *unit = new ZergUnit(entityData.name, resourceManager);
					units.push_back(unit);
					resourceManager.addSupplyMax(entityDataMap.at(entityData.name).supplyProvided);
				}
				//remove this larva
				it = larvas.erase(it);
			} else {
				++it;
			}
		}
		for (ZergUpgradeableUnit *u : upgradeableUnits) {
			u->update();//TODO
		}
		for (ZergDrone *u : drones) {
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
