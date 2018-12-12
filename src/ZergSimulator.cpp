#include "../include/ForwardSimulator.h"
#include "../include/EntityData.h"

#include <iostream>
#include <algorithm>
#include <sstream>

using std::cout;
using std::endl;
using std::stringstream;

ZergSimulator::ZergSimulator() : timestep(1), maxTime(200), running(true), gasBuildings(0) {
	init();
}

ZergSimulator::ZergSimulator(queue<string> q) : buildOrder(q), timestep(1), maxTime(200), running(true), gasBuildings(0) {
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
	
	resourceManager.consumeSupply(6); //6 drones cost
	resourceManager.setMineralWorkers(6);
	resourceManager.setVespeneWorkers(0);
	
	cout << endl;
}

void ZergSimulator::simulate() {
	while (running && (timestep < maxTime)) {//TODO
		
		stringstream ss;
		cout << "t = " << timestep << endl;
		
		resourceManager.update();
		cout << resourceManager << endl;
		
		//update/finish buildings
		bool buildFinished = false;
		
		for (ZergHatchery *b : hatcheries) {
			b->update();
			if (b->takeQueen()) {
				ZergQueen *queen = new ZergQueen(string("queen"), resourceManager);
				queens.push_back(queen);
				buildFinished = true;
			}
		}
		for (ZergSpire *b : spires) {
			b->update();
			//TODO if upgrade done generate event
		}
		for (ZergNydusNetwork *b : nydusNetworks) {
			b->update();
			if (b->takeUnit()) {
				ZergUnit *nydusWorm = new ZergUnit(string("nydus_worm"), resourceManager);
				units.push_back(nydusWorm);
				buildFinished = true;
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
					buildFinished = true;
				} else if ((entityData.name == string("overlord")) || (entityData.name == string("zergling")) || (entityData.name == string("corruptor"))) {
					ZergUpgradeableUnit *unit = new ZergUpgradeableUnit(entityData.name, resourceManager);
					upgradeableUnits.push_back(unit);
					buildFinished = true;
				} else {
					ZergUnit *unit = new ZergUnit(entityData.name, resourceManager);
					units.push_back(unit);
					buildFinished = true;
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
			//TODO if upgrade done generate event
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
					buildFinished = true;
				} else if (entityData.name == string("spire")) {
					ZergSpire *spire = new ZergSpire(string("spire"), resourceManager);
					spires.push_back(spire);
					buildFinished = true;
				} else if (entityData.name == string("nydus_network")) {
					ZergNydusNetwork *nydusNetwork = new ZergNydusNetwork(string("nydus_network"), resourceManager);
					nydusNetworks.push_back(nydusNetwork);
					buildFinished = true;
				} else {
					if (entityData.name == string("extractor")) {
						++gasBuildings;
					}
					ZergBuilding *building = new ZergBuilding(entityData.name, resourceManager);
					buildings.push_back(building);
					buildFinished = true;
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
						//TODO generate event
						break;
					}
				}
			}
		}
		
		//start build if possible //TODO
		bool removeItem = false;
		
		if (!buildOrder.empty()) {
			string nextItem = buildOrder.front();
			EntityData& entityData = entityDataMap.at(nextItem);
			
			
			if (nextItem == string("overseer")) { //unit upgrade
				for (auto *u : upgradeableUnits) {
					if (u->getName() == string("overlord")) {
						if (u->upgrade()) {
							removeItem = true;
							break;
						}
					}
				}
			} else if (nextItem == string("baneling")) {
				for (auto *u : upgradeableUnits) {
					if (u->getName() == string("zergling")) {
						if (u->upgrade()) {
							removeItem = true;
							break;
						}
					}
				}
			} else if (nextItem == string("brood_lord")) {
				for (auto *u : upgradeableUnits) {
					if (u->getName() == string("corruptor")) {
						if (u->upgrade()) {
							removeItem = true;
							break;
						}
					}
				}
			} else if (nextItem == string("lair")) { //building upgrade
				for (auto *b : hatcheries) {
					if (b->getName() == string("hatchery")) {
						if (b->upgrade()) {
							removeItem = true;
							break;
						}
					}
				}
			} else if (nextItem == string("hive")) {
				for (auto *b : hatcheries) {
					if (b->getName() == string("lair")) {
						if (b->upgrade()) {
							removeItem = true;
							break;
						}
					}
				}
			} else if (nextItem == string("greater_spire")) {
				for (auto *s : spires) {
					if (s->upgrade()) {
						removeItem = true;
						break;
					}
				}
			} else if (nextItem == string("nydus_worm")) { //building produce unit
				for (auto *n : nydusNetworks) {
					if (n->spawn()) {
						removeItem = true;
						break;
					}
				}
			} else if (nextItem == string("queen")) {
				for (auto *b : hatcheries) {
					if (b->spawnQueen()) {
						removeItem = true;
						break;
					}
				}
			} else if (entityData.isBuilding) { //drone to building // TODO check if can build gas building ??
				for (auto *d : drones) {
					if (d->morph(entityData)) {
						removeItem = true;
						break;
					}
				}
			} else { //larva to unit
				for (auto *b : hatcheries) {
					if (b->morphLarva(entityData)) {
						ZergLarva *larva = new ZergLarva(string("larva"), resourceManager, entityData);
						larvas.push_back(larva);
						removeItem = true;
						break;
					}
				}
			}
			
			
			if (removeItem) {
				buildOrder.pop();
			}
			
		}
		
		
		//redistribute workers
		//TODO
		int workers = 0;
		int maxGasWorkers = gasBuildings * 3;
		for (auto *d : drones) {
			if (!d->isMorphing()) {
				++workers;
			}
		}
		int gasWorkers = std::min(workers, maxGasWorkers);
		int mineralWorkers = workers - gasWorkers;
		
		resourceManager.setMineralWorkers(mineralWorkers);
		resourceManager.setVespeneWorkers(gasWorkers);
		
		if (removeItem || buildFinished) {
			
		} else {
			
		}
		
		
		++timestep;
		
		
	}
}






