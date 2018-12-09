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
	hatcheries.push_back(hatchery);
	
	ZergUpgradeableUnit *overlord = new ZergUpgradeableUnit(string("overlord"), resourceManager);
	upgradeableUnits.push_back(overlord);
	
	for (int i = 0; i < 6; ++i) {
		ZergDrone *drone = new ZergDrone(string("drone"), resourceManager);
		//drone.setWorking(true);
		drones.push_back(drone);
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
			if (b->takeQueen()) {
				ZergQueen *queen = new ZergQueen(string("queen"), resourceManager);
				queens.push_back(queen);
			}
		}
		for (ZergSpire *b : spires) {
			b->update();
		}
		for (ZergNydusNetwork *b : nydusNetworks) {
			b->update();
			if (b->takeUnit()) {
				ZergUnit *nydusWorm = new ZergUnit(string("nydus_worm"), resourceManager);
				units.push_back(nydusWorm);
			}
		}
		
		//update/finish units
		auto it = larvas.begin();
		while (it != larvas.end()) {
			ZergLarva *larva = (*it);
			larva->update();
			if (larva->isDone()) {
				EntityData& entityData = larva->getUnitData();
				//create new unit
				if (entityData.name == string("drone")) {
					ZergDrone *drone = new ZergDrone(string("drone"), resourceManager);
					drones.push_back(drone);
				} else if ((entityData.name == string("overlord")) || (entityData.name == string("zergling")) || (entityData.name == string("corruptor"))) {
					ZergUpgradeableUnit *unit = new ZergUpgradeableUnit(entityData.name, resourceManager);
					upgradeableUnits.push_back(unit);
				} else {
					ZergUnit *unit = new ZergUnit(entityData.name, resourceManager);
					units.push_back(unit);
				}
				//remove this larva
				it = larvas.erase(it);
				delete larva;
			} else {
				++it;
			}
		}
		
		for (ZergUpgradeableUnit *u : upgradeableUnits) {
			u->update();
		}
		
		auto it2 = drones.begin();
		while (it2 != drones.end()) {
			ZergDrone *drone = (*it2);
			drone->update();
			if (drone->morphingDone()) {
				EntityData& entityData = drone->getBuildingData();
				//create new unit
				if (entityData.name == string("hatchery")) {
					ZergHatchery *hatchery = new ZergHatchery(string("hatchery"), resourceManager);
					hatcheries.push_back(hatchery);
				} else if (entityData.name == string("spire")) {
					ZergSpire *spire = new ZergSpire(string("spire"), resourceManager);
					spires.push_back(spire);
				} else if (entityData.name == string("nydus_network")) {
					ZergNydusNetwork *nydusNetwork = new ZergNydusNetwork(string("nydus_network"), resourceManager);
					nydusNetworks.push_back(nydusNetwork);
				} else {
					ZergBuilding *building = new ZergBuilding(entityData.name, resourceManager);
					buildings.push_back(building);
				}
				//remove this drone
				it2 = drones.erase(it2);
				delete drone;
			} else {
				++it2;
			}
		}
		
		for (ZergQueen *u : queens) {
			u->update();
		}
		
		//start/finish abilities
		for (ZergQueen *queen : queens) {
			if (queen->canInjectLarvas()) {
				//search for a hatchery
				for (ZergHatchery *hatchery : hatcheries) {
					if (hatchery->injectLarvas()) {
						queen->injectLarvas();
						break;
					}
				}
			}
		}
		
		//start build if possible //TODO
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
		//TODO
		
		++timestep;
	}
}
