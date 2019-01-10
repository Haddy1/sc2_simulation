#include "../include/ForwardSimulator.h"
#include "../include/EntityData.h"

#include <iostream>
#include <algorithm>
#include <sstream>

using std::cout;
using std::endl;
using std::stringstream;
using std::to_string;

ZergSimulator::ZergSimulator(bool logging, int maxTime) : logger(ZERG, resourceManager, true), maxTime(maxTime), gasBuildings(0), busyCounter(0), ID_Counter(0), logging(logging), timestep(1), timedOut(false) {
	
}

ZergSimulator::ZergSimulator(queue<string> q, bool logging, int maxTime) : logger(ZERG, resourceManager, true), buildOrder(q), maxTime(maxTime), gasBuildings(0), busyCounter(0), ID_Counter(0), logging(logging), timestep(1), timedOut(false) {
	
}

ZergSimulator::~ZergSimulator() {
	
}

void ZergSimulator::init() {
	//cout << "Start Configuration:" << endl;
	vector<int> droneIDs;
	vector<int> hatcheryIDs;
	vector<int> overlordIDs;
	
	ZergHatchery hatchery(ID_Counter, string("hatchery"), resourceManager, tech, busyCounter);
	hatcheryIDs.push_back(hatchery.getID());
	hatcheries.push_back(hatchery);
	
	ZergUpgradeableUnit overlord(ID_Counter, string("overlord"), resourceManager, tech, busyCounter);
	overlordIDs.push_back(overlord.getID());
	upgradeableUnits.push_back(overlord);
	
	
	
	for (int i = 0; i < 6; ++i) {
		ZergDrone drone(ID_Counter, string("drone"), resourceManager, tech, busyCounter);
		droneIDs.push_back(drone.getID());
		drones.push_back(drone);
	}
	
	resourceManager.consumeSupply(6); //6 drones cost
	resourceManager.setMineralWorkers(6);
	resourceManager.setVespeneWorkers(0);
	
	vector<pair<string, vector<int>>> initUnits;
	initUnits.push_back(pair<string, vector<int>>(string("drone"), droneIDs));
	initUnits.push_back(pair<string, vector<int>>(string("hatchery"), hatcheryIDs));
	initUnits.push_back(pair<string, vector<int>>(string("overlord"), overlordIDs));
	
	if (logging) {
		logger.printBeginning();
		logger.printSetup(initUnits);
	}
	//logger.printMessage(0);
	
	//cout << endl;
}

void ZergSimulator::simulate() {
	bool continueSimulation = true;
	
	while (continueSimulation && (timestep <= maxTime)) {
		
		
		/*
		* Update Resources
		*/
		resourceManager.update();
		
		/*
		* Update/Finish Buildings
		*/
		
		//update queens first to avoid update directly after a hatchery spawns a queen
		for (ZergQueen& u : queens) {
			u.update();
		}
		
		for (ZergHatchery& b : hatcheries) {
			if (b.update()) { //Upgrade done, generate event
				if (logging)
					logger.addBuildend(BuildEndEntry(b.getName(), b.getID(), b.getID()));
			}
			if (b.takeQueen()) {
				ZergQueen queen(ID_Counter, string("queen"), resourceManager, busyCounter);
				queens.push_back(queen);
				if (logging)
					logger.addBuildend(BuildEndEntry("queen", b.getID(), queen.getID()));
			}
			//std::clog << "Hatchery num larvas: " << b.getLarvaCount() << std::endl;
		}
		for (ZergSpire& b : spires) {
			if (b.update()) { //Upgrade done, generate event
				if (logging)
					logger.addBuildend(BuildEndEntry(b.getName(), b.getID(), b.getID()));
			}
		}
		for (ZergNydusNetwork& b : nydusNetworks) {
			b.update();
			if (b.takeUnit()) {
				ZergUnit nydusWorm(ID_Counter, string("nydus_worm"), resourceManager);
				units.push_back(nydusWorm);
				if (logging)
					logger.addBuildend(BuildEndEntry("nydus_worm", b.getID(), nydusWorm.getID()));
			}
		}
		
		//update/finish units
		for (ZergUpgradeableUnit& u : upgradeableUnits) {
			if (u.update()) { //Upgrade done, generate event
				if (logging)
					logger.addBuildend(BuildEndEntry(u.getName(), u.getID(), u.getID()));
			}
		}
		
		//Drones
		auto it2 = drones.begin();
		while (it2 != drones.end()) {
			ZergDrone& drone = (*it2);
			drone.update();
			if (drone.morphingDone()) {
				EntityData *entityData = drone.getBuildingData();
				int producerID = drone.getID();
				int producedIDs;
				//create new unit
				if (entityData->name == string("hatchery")) {
					ZergHatchery hatchery(ID_Counter, string("hatchery"), resourceManager, tech, busyCounter);
					hatcheries.push_back(hatchery);
					producedIDs = hatchery.getID();
				} else if (entityData->name == string("spire")) {
					ZergSpire spire(ID_Counter, string("spire"), resourceManager, tech, busyCounter);
					spires.push_back(spire);
					producedIDs = spire.getID();
				} else if (entityData->name == string("nydus_network")) {
					ZergNydusNetwork nydusNetwork(ID_Counter, string("nydus_network"), resourceManager, tech, busyCounter);
					nydusNetworks.push_back(nydusNetwork);
					producedIDs = nydusNetwork.getID();
				} else {
					if (entityData->name == string("extractor")) {
						++gasBuildings;
					}
					ZergBuilding building(ID_Counter, entityData->name, resourceManager, tech);
					buildings.push_back(building);
					producedIDs = building.getID();
				}
				
				if (logging)
					logger.addBuildend(BuildEndEntry(entityData->name, producerID, producedIDs));
				//remove this drone
				it2 = drones.erase(it2);
				--busyCounter; //drone done, so decrease counter
			} else {
				++it2;
			}
		}
		
		//Larvas
		auto it = larvas.begin();
		while (it != larvas.end()) {
			ZergLarva& larva = (*it);
			larva.update();
			if (larva.isDone()) {
				EntityData *entityData = larva.getUnitData();
				string producerID = to_string(larva.getID());
				vector<string> producedIDs;
				//create new unit
				if (entityData->name == string("drone")) {
					ZergDrone drone(ID_Counter, string("drone"), resourceManager, tech, busyCounter);
					drones.push_back(drone);
					producedIDs.push_back(to_string(drone.getID()));
				} else if (entityData->name == string("zergling")) { //edge case: 2 zerglings are produced
					ZergUpgradeableUnit unit1(ID_Counter, entityData->name, resourceManager, tech, busyCounter);
					ZergUpgradeableUnit unit2(ID_Counter, entityData->name, resourceManager, tech, busyCounter);
					upgradeableUnits.push_back(unit1);
					upgradeableUnits.push_back(unit2);
					producedIDs.push_back(to_string(unit1.getID()));
					producedIDs.push_back(to_string(unit2.getID()));
				} else if ((entityData->name == string("overlord")) || (entityData->name == string("corruptor"))) {
					ZergUpgradeableUnit unit(ID_Counter, entityData->name, resourceManager, tech, busyCounter);
					upgradeableUnits.push_back(unit);
					producedIDs.push_back(to_string(unit.getID()));
				} else {
					ZergUnit unit(ID_Counter, entityData->name, resourceManager);
					units.push_back(unit);
					producedIDs.push_back(to_string(unit.getID()));
				}
				
				if (logging)
					logger.addBuildend(BuildEndEntry(entityData->name, producerID, producedIDs));
				//remove this larva
				it = larvas.erase(it);
				--busyCounter; //larva done, so decrease counter
			} else {
				++it;
			}
		}
		
		
		
		
		//TODO updateAbility function for hatchery?
		/*
		* Start/Finish Abilities
		*/
		bool abilityStarted = false;
		
		for (ZergQueen& queen : queens) {
			if (queen.canInjectLarvas()) {
				//search for a hatchery
				for (ZergHatchery& hatchery : hatcheries) {
					if (hatchery.injectLarvas()) {
						queen.injectLarvas();
						abilityStarted = true;
						int hatcheryID = hatchery.getID();
						int queenID = queen.getID();
						
						if (logging)
							logger.addSpecial(SpecialEntry("injectlarvae", queenID, hatcheryID));
						break;
					}
				}
			}
		}
		
		
		/*
		* Start Build if possible
		*/
		bool buildStarted = false;
		
		if ((!buildOrder.empty()) && (!abilityStarted)) {
			string nextItem = buildOrder.front();
			EntityData& entityData = entityDataMap.at(nextItem);
			
			int producerID = -1;
			
			
			if (nextItem == string("overseer")) { //unit upgrade
				for (auto& u : upgradeableUnits) {
					if (u.getName() == string("overlord")) {
						if (u.upgrade()) {
							buildStarted = true;
							producerID = u.getID();
							break;
						}
					}
				}
			} else if (nextItem == string("baneling")) {
				for (auto& u : upgradeableUnits) {
					if (u.getName() == string("zergling")) {
						if (u.upgrade()) {
							buildStarted = true;
							producerID = u.getID();
							break;
						}
					}
				}
			} else if (nextItem == string("brood_lord")) {
				for (auto& u : upgradeableUnits) {
					if (u.getName() == string("corruptor")) {
						if (u.upgrade()) {
							buildStarted = true;
							producerID = u.getID();
							break;
						}
					}
				}
			} else if (nextItem == string("lair")) { //building upgrade
				for (auto& b : hatcheries) {
					if (b.getName() == string("hatchery")) {
						if (b.upgrade()) {
							buildStarted = true;
							producerID = b.getID();
							break;
						}
					}
				}
			} else if (nextItem == string("hive")) {
				for (auto& b : hatcheries) {
					if (b.getName() == string("lair")) {
						if (b.upgrade()) {
							buildStarted = true;
							producerID = b.getID();
							break;
						}
					}
				}
			} else if (nextItem == string("greater_spire")) {
				for (auto& s : spires) {
					if (s.upgrade()) {
						buildStarted = true;
						producerID = s.getID();
						break;
					}
				}
			} else if (nextItem == string("nydus_worm")) { //building produce unit
				for (auto& n : nydusNetworks) {
					if (n.spawn()) {
						buildStarted = true;
						producerID = n.getID();
						break;
					}
				}
			} else if (nextItem == string("queen")) {
				for (auto& b : hatcheries) {
					if (b.spawnQueen()) {
						buildStarted = true;
						producerID = b.getID();
						break;
					}
				}
			} else if (entityData.isBuilding) { //drone to building // TODO check if can build gas building ?? (max 2)
				for (auto& d : drones) {
					if (d.morph(&entityData)) {
						buildStarted = true;
						producerID = d.getID();
						++busyCounter; //started a morph
						break;
					}
				}
			} else { //larva to unit
				for (auto& b : hatcheries) {
					if (b.morphLarva(entityData)) {
						ZergLarva larva(ID_Counter, string("larva"), resourceManager, &entityData, busyCounter);
						larvas.push_back(larva);
						buildStarted = true;
						producerID = -1; //if larva morphs, dont give a producerID
						++busyCounter; //started a morph
						break;
					}
				}
			}
			
			
			if (buildStarted) {
				if (logging) {
					if (producerID == -1) {
						logger.addBuildstart(BuildStartEntry(nextItem));
					} else {
						logger.addBuildstart(BuildStartEntry(nextItem, producerID));
					}
				}
				buildOrder.pop();
			}
			
		}
		
		
		/*
		* Redistribute Workers
		*/
		int workers = 0;
		int maxGasWorkers = gasBuildings * 3;
		for (auto& d : drones) {
			if (!d.isMorphing()) {
				++workers;
			}
		}
		int gasWorkers = std::min(workers, maxGasWorkers);
		int mineralWorkers = workers - gasWorkers;
		
		resourceManager.setMineralWorkers(mineralWorkers);
		resourceManager.setVespeneWorkers(gasWorkers);
		
		
		
		/*
		* End simulation if everything idle and queue empty
		*/
		continueSimulation = (!buildOrder.empty());
		continueSimulation = continueSimulation || (busyCounter > 0);
		
		//temp
		//std::clog << "busy: " << busyCounter << std::endl;
		
		
		if (logging)
			logger.printMessage(timestep);
		
		
		++timestep;
		
		
	}
	
	if (logging)
		logger.printEnd();
	
	
	
	//if (!continueSimulation) {
	//	std::clog << "EVERYTHING IDLE at t = " << timestep << std::endl;
	//}
	if (timestep > maxTime) {
		timedOut = true;
	}
	
	
}

int ZergSimulator::numberOfUnits(string unitname) {
	//TODO
	return 0;
}

int ZergSimulator::numberOfWorkers() {
	//TODO
	return 0;
}

int ZergSimulator::numberOfProductionStructures() {
	//TODO
	return 0;
}






