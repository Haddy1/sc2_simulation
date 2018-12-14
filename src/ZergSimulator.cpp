#include "../include/ForwardSimulator.h"
#include "../include/EntityData.h"

#include <iostream>
#include <algorithm>
#include <sstream>

using std::cout;
using std::endl;
using std::stringstream;

ZergSimulator::ZergSimulator() : logger(ZERG, resourceManager, true), timestep(1), maxTime(600), gasBuildings(0) {
	init();
}

ZergSimulator::ZergSimulator(queue<string> q) : logger(PROTOSS, resourceManager, true), buildOrder(q), timestep(1), maxTime(600), gasBuildings(0) {
	init();
}

void ZergSimulator::init() {
	//cout << "Start Configuration:" << endl;
	vector<int> droneIDs;
	vector<int> hatcheryIDs;
	vector<int> overlordIDs;
	
	ZergHatchery *hatchery = new ZergHatchery(string("hatchery"), resourceManager);
	hatcheryIDs.push_back(hatchery->getID());
	hatcheries.push_back(hatchery);
	
	ZergUpgradeableUnit *overlord = new ZergUpgradeableUnit(string("overlord"), resourceManager);
	overlordIDs.push_back(overlord->getID());
	upgradeableUnits.push_back(overlord);
	
	
	
	for (int i = 0; i < 6; ++i) {
		ZergDrone *drone = new ZergDrone(string("drone"), resourceManager);
		droneIDs.push_back(drone->getID());
		//drone.setWorking(true);
		drones.push_back(drone);
	}
	
	resourceManager.consumeSupply(6); //6 drones cost
	resourceManager.setMineralWorkers(6);
	resourceManager.setVespeneWorkers(0);
	
	vector<pair<string, vector<int>>> initUnits;
	//pair<string, vector<int>> dronePair(string("drone"), droneIDs);
	//initUnits.push_back(make_pair<string, vector<int>>(stringDrone, droneIDs));
	//initUnits.push_back(dronePair);
	initUnits.push_back(pair<string, vector<int>>(string("drone"), droneIDs));
	initUnits.push_back(pair<string, vector<int>>(string("hatchery"), hatcheryIDs));
	initUnits.push_back(pair<string, vector<int>>(string("overlord"), overlordIDs));
	
	//vector<pair<string, vector<int>>> initUnits = {
	//	make_pair<string, vector<int>>(string("drone"), droneIDs),
	//	make_pair<string, vector<int>>(string("hatchery"), hatcheryIDs),
	//	make_pair<string, vector<int>>(string("overlord"), overlordIDs)
	//};
	
	logger.printSetup(initUnits);
	logger.printMessage(0, {});
	
	//cout << endl;
}

void ZergSimulator::simulate() {
	bool continueSimulation = true;
	
	while (continueSimulation && (timestep < maxTime)) {//TODO
		
		vector<EventEntry*> loggerEvents;
		//cout << "t = " << timestep << endl;
		
		/*
		* Update Resources
		*/
		resourceManager.update();
		//cout << resourceManager << endl;
		
		/*
		* Update/Finish Buildings
		*/
		bool buildFinished = false;
		
		//update queens first to avoid update directly after a hatchery spawns a queen
		for (ZergQueen *u : queens) {
			u->update();
		}
		
		for (ZergHatchery *b : hatcheries) {
			b->update();
			if (b->takeQueen()) {
				ZergQueen *queen = new ZergQueen(string("queen"), resourceManager);
				queens.push_back(queen);
				EventEntry *eventEntry = new EventEntry("build-end", "queen");
				loggerEvents.push_back(eventEntry);
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
				EventEntry *eventEntry = new EventEntry("build-end", "nydus_worm");
				loggerEvents.push_back(eventEntry);
				buildFinished = true;
			}
		}
		
		//update/finish units
		for (ZergUpgradeableUnit *u : upgradeableUnits) {
			u->update();
			//TODO if upgrade done generate event
		}
		
		//Drones
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
					EventEntry *eventEntry = new EventEntry("build-end", entityData.name);
					loggerEvents.push_back(eventEntry);
					buildFinished = true;
				} else if (entityData.name == string("spire")) {
					ZergSpire *spire = new ZergSpire(string("spire"), resourceManager);
					spires.push_back(spire);
					EventEntry *eventEntry = new EventEntry("build-end", entityData.name);
					loggerEvents.push_back(eventEntry);
					buildFinished = true;
				} else if (entityData.name == string("nydus_network")) {
					ZergNydusNetwork *nydusNetwork = new ZergNydusNetwork(string("nydus_network"), resourceManager);
					nydusNetworks.push_back(nydusNetwork);
					EventEntry *eventEntry = new EventEntry("build-end", entityData.name);
					loggerEvents.push_back(eventEntry);
					buildFinished = true;
				} else {
					if (entityData.name == string("extractor")) {
						++gasBuildings;
					}
					ZergBuilding *building = new ZergBuilding(entityData.name, resourceManager);
					buildings.push_back(building);
					EventEntry *eventEntry = new EventEntry("build-end", entityData.name);
					loggerEvents.push_back(eventEntry);
					buildFinished = true;
				}
				//remove this drone
				it2 = drones.erase(it2);
				delete drone;
			} else {
				++it2;
			}
		}
		
		//Larvas
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
					EventEntry *eventEntry = new EventEntry("build-end", entityData.name);
					loggerEvents.push_back(eventEntry);
					buildFinished = true;
				} else if ((entityData.name == string("overlord")) || (entityData.name == string("zergling")) || (entityData.name == string("corruptor"))) {
					ZergUpgradeableUnit *unit = new ZergUpgradeableUnit(entityData.name, resourceManager);
					upgradeableUnits.push_back(unit);
					EventEntry *eventEntry = new EventEntry("build-end", entityData.name);
					loggerEvents.push_back(eventEntry);
					buildFinished = true;
				} else {
					ZergUnit *unit = new ZergUnit(entityData.name, resourceManager);
					units.push_back(unit);
					EventEntry *eventEntry = new EventEntry("build-end", entityData.name); // dont copy these 2 lines over and over
					loggerEvents.push_back(eventEntry);
					buildFinished = true;
				}
				//remove this larva
				it = larvas.erase(it);
				delete larva;
			} else {
				++it;
			}
		}
		
		
		
		
		
		/*
		* Start/Finish Abilities
		*/
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
		
		
		/*
		* Start Build if possible
		*/
		//start build if possible //TODO
		bool buildStarted = false;
		
		if (!buildOrder.empty()) {
			string nextItem = buildOrder.front();
			EntityData& entityData = entityDataMap.at(nextItem);
			
			
			if (nextItem == string("overseer")) { //unit upgrade
				for (auto *u : upgradeableUnits) {
					if (u->getName() == string("overlord")) {
						if (u->upgrade()) {
							buildStarted = true;
							break;
						}
					}
				}
			} else if (nextItem == string("baneling")) {
				for (auto *u : upgradeableUnits) {
					if (u->getName() == string("zergling")) {
						if (u->upgrade()) {
							buildStarted = true;
							break;
						}
					}
				}
			} else if (nextItem == string("brood_lord")) {
				for (auto *u : upgradeableUnits) {
					if (u->getName() == string("corruptor")) {
						if (u->upgrade()) {
							buildStarted = true;
							break;
						}
					}
				}
			} else if (nextItem == string("lair")) { //building upgrade
				for (auto *b : hatcheries) {
					if (b->getName() == string("hatchery")) {
						if (b->upgrade()) {
							buildStarted = true;
							break;
						}
					}
				}
			} else if (nextItem == string("hive")) {
				for (auto *b : hatcheries) {
					if (b->getName() == string("lair")) {
						if (b->upgrade()) {
							buildStarted = true;
							break;
						}
					}
				}
			} else if (nextItem == string("greater_spire")) {
				for (auto *s : spires) {
					if (s->upgrade()) {
						buildStarted = true;
						break;
					}
				}
			} else if (nextItem == string("nydus_worm")) { //building produce unit
				for (auto *n : nydusNetworks) {
					if (n->spawn()) {
						buildStarted = true;
						break;
					}
				}
			} else if (nextItem == string("queen")) {
				for (auto *b : hatcheries) {
					if (b->spawnQueen()) {
						buildStarted = true;
						break;
					}
				}
			} else if (entityData.isBuilding) { //drone to building // TODO check if can build gas building ?? (max 2)
				for (auto *d : drones) {
					if (d->morph(entityData)) {
						buildStarted = true;
						break;
					}
				}
			} else { //larva to unit
				for (auto *b : hatcheries) {
					if (b->morphLarva(entityData)) {
						ZergLarva *larva = new ZergLarva(string("larva"), resourceManager, entityData);
						larvas.push_back(larva);
						buildStarted = true;
						break;
					}
				}
			}
			
			
			if (buildStarted) {
				EventEntry *eventEntry = new EventEntry("build-start", nextItem);
				loggerEvents.push_back(eventEntry);
				
				buildOrder.pop();
			}
			
		}
		
		
		/*
		* Redistribute Workers
		*/
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
		
		
		
		/*
		* TODO End simulation if everything idle and queue empty
		*/
		continueSimulation = (!buildOrder.empty());
		for (auto *a : hatcheries) {
			continueSimulation |= a->busy();
		}
		for (auto *a : spires) {
			continueSimulation |= a->busy();
		}
		for (auto *a : nydusNetworks) {
			continueSimulation |= a->busy();
		}
		for (auto *a : upgradeableUnits) {
			continueSimulation |= a->busy();
		}
		for (auto *a : drones) {
			continueSimulation |= a->busy();
		}
		for (auto *a : larvas) {
			continueSimulation |= a->busy();
		}
		for (auto *a : queens) {
			continueSimulation |= a->busy();
		}
		
		
		
		
		//AbilityEntry c("special", "chronoboost");
		//loggerEvents.push_back(&c);
		if (buildFinished) {
			//TODO unused var
		}
		
		
		
		logger.printMessage(timestep, loggerEvents);
		for (auto it = loggerEvents.begin(); it != loggerEvents.end(); ++it) {
			delete (*it);
		}
		
		
		++timestep;
		
		
	}
}






