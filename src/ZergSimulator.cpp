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

ZergSimulator::ZergSimulator(queue<EntityType> q, bool logging, int maxTime) : logger(ZERG, resourceManager, true), buildOrder(q), maxTime(maxTime), gasBuildings(0), busyCounter(0), ID_Counter(0), logging(logging), timestep(1), timedOut(false) {
	
}

ZergSimulator::~ZergSimulator() {
	
}

void ZergSimulator::init() {
	//cout << "Start Configuration:" << endl;
	vector<int> droneIDs;
	vector<int> hatcheryIDs;
	vector<int> overlordIDs;
	
	ZergHatchery hatcheryx(ID_Counter, hatchery, resourceManager, tech, busyCounter);
	hatcheryIDs.push_back(hatcheryx.getID());
	hatcheries.push_back(hatcheryx);
	
	ZergUpgradeableUnit overlordx(ID_Counter, overlord, resourceManager, tech, busyCounter);
	overlordIDs.push_back(overlordx.getID());
	upgradeableUnits.push_back(overlordx);
	
	
	
	for (int i = 0; i < 6; ++i) {
		ZergDrone dronex(ID_Counter, drone, resourceManager, tech, busyCounter);
		droneIDs.push_back(dronex.getID());
		drones.push_back(dronex);
	}
	
	resourceManager.consumeSupply(6); //6 drones cost
	resourceManager.setMineralWorkers(6);
	resourceManager.setVespeneWorkers(0);
	
	
	if (logging) {
		vector<pair<string, vector<int>>> initUnits;
		initUnits.push_back(pair<string, vector<int>>(string("drone"), droneIDs));
		initUnits.push_back(pair<string, vector<int>>(string("hatchery"), hatcheryIDs));
		initUnits.push_back(pair<string, vector<int>>(string("overlord"), overlordIDs));
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
				ZergQueen queenx(ID_Counter, queen, resourceManager, busyCounter);
				queens.push_back(queenx);
				if (logging)
					logger.addBuildend(BuildEndEntry("queen", b.getID(), queenx.getID()));
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
				ZergUnit nydusWorm(ID_Counter, nydus_worm, resourceManager);
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
			ZergDrone& dronex = (*it2);
			dronex.update();
			if (dronex.morphingDone()) {
				EntityData *entityData = dronex.getBuildingData();
				int producerID = dronex.getID();
				int producedIDs;
				//create new unit
				if (entityData->type == hatchery) {
					ZergHatchery hatcheryx(ID_Counter, hatchery, resourceManager, tech, busyCounter);
					hatcheries.push_back(hatcheryx);
					producedIDs = hatcheryx.getID();
				} else if (entityData->type == spire) {
					ZergSpire spirex(ID_Counter, spire, resourceManager, tech, busyCounter);
					spires.push_back(spirex);
					producedIDs = spirex.getID();
				} else if (entityData->type == nydus_network) {
					ZergNydusNetwork nydusNetworkx(ID_Counter, nydus_network, resourceManager, tech, busyCounter);
					nydusNetworks.push_back(nydusNetworkx);
					producedIDs = nydusNetworkx.getID();
				} else {
					if (entityData->type == extractor) {
						++gasBuildings;
					}
					ZergBuilding buildingx(ID_Counter, entityData->type, resourceManager, tech);
					buildings.push_back(buildingx);
					producedIDs = buildingx.getID();
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
			ZergLarva& larvax = (*it);
			larvax.update();
			if (larvax.isDone()) {
				EntityData *entityData = larvax.getUnitData();
				
				vector<string> producedIDs;
				//create new unit
				if (entityData->type == drone) {
					ZergDrone dronex(ID_Counter, drone, resourceManager, tech, busyCounter);
					drones.push_back(dronex);
					if (logging) {
						producedIDs.push_back(to_string(dronex.getID()));
					}
				} else if (entityData->type == zergling) { //edge case: 2 zerglings are produced
					ZergUpgradeableUnit unit1x(ID_Counter, entityData->type, resourceManager, tech, busyCounter);
					ZergUpgradeableUnit unit2x(ID_Counter, entityData->type, resourceManager, tech, busyCounter);
					upgradeableUnits.push_back(unit1x);
					upgradeableUnits.push_back(unit2x);
					if (logging) {
						producedIDs.push_back(to_string(unit1x.getID()));
						producedIDs.push_back(to_string(unit2x.getID()));
					}
				} else if ((entityData->type == overlord) || (entityData->type == corruptor)) {
					ZergUpgradeableUnit unitx(ID_Counter, entityData->type, resourceManager, tech, busyCounter);
					upgradeableUnits.push_back(unitx);
					if (logging) {
						producedIDs.push_back(to_string(unitx.getID()));
					}
				} else {
					ZergUnit unitx(ID_Counter, entityData->type, resourceManager);
					units.push_back(unitx);
					if (logging) {
						producedIDs.push_back(to_string(unitx.getID()));
					}
				}
				
				if (logging) {
					string producerID = to_string(larvax.getID());
					logger.addBuildend(BuildEndEntry(entityData->name, producerID, producedIDs));
				}
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
		
		for (ZergQueen& queenx : queens) {
			if (queenx.canInjectLarvas()) {
				//search for a hatchery
				for (ZergHatchery& hatchery : hatcheries) {
					if (hatchery.injectLarvas()) {
						queenx.injectLarvas();
						abilityStarted = true;
						int hatcheryID = hatchery.getID();
						int queenID = queenx.getID();
						
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
			EntityType nextItem = buildOrder.front();
			EntityData& entityData = entityDataMap.at(nextItem);
			
			int producerID = -1;
			
			
			if (nextItem == overseer) { //unit upgrade
				for (auto& u : upgradeableUnits) {
					if (u.getType() == overlord) {
						if (u.upgrade()) {
							buildStarted = true;
							producerID = u.getID();
							break;
						}
					}
				}
			} else if (nextItem == baneling) {
				for (auto& u : upgradeableUnits) {
					if (u.getType() == zergling) {
						if (u.upgrade()) {
							buildStarted = true;
							producerID = u.getID();
							break;
						}
					}
				}
			} else if (nextItem == brood_lord) {
				for (auto& u : upgradeableUnits) {
					if (u.getType() == corruptor) {
						if (u.upgrade()) {
							buildStarted = true;
							producerID = u.getID();
							break;
						}
					}
				}
			} else if (nextItem == lair) { //building upgrade
				for (auto& b : hatcheries) {
					if (b.getType() == hatchery) {
						if (b.upgrade()) {
							buildStarted = true;
							producerID = b.getID();
							break;
						}
					}
				}
			} else if (nextItem == hive) {
				for (auto& b : hatcheries) {
					if (b.getType() == lair) {
						if (b.upgrade()) {
							buildStarted = true;
							producerID = b.getID();
							break;
						}
					}
				}
			} else if (nextItem == greater_spire) {
				for (auto& s : spires) {
					if (s.upgrade()) {
						buildStarted = true;
						producerID = s.getID();
						break;
					}
				}
			} else if (nextItem == nydus_worm) { //building produce unit
				for (auto& n : nydusNetworks) {
					if (n.spawn()) {
						buildStarted = true;
						producerID = n.getID();
						break;
					}
				}
			} else if (nextItem == queen) {
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
						ZergLarva larvax(ID_Counter, larva, resourceManager, &entityData, busyCounter);
						larvas.push_back(larvax);
						buildStarted = true;
						producerID = -1; //if larva morphs, dont give a producerID
						++busyCounter; //started a morph
						break;
					}
				}
			}
			
			
			
			if (buildStarted) {
				if (logging) {
					string nextItemString = entityNameMap.at(nextItem);
					if (producerID == -1) {
						logger.addBuildstart(BuildStartEntry(nextItemString));
					} else {
						logger.addBuildstart(BuildStartEntry(nextItemString, producerID));
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

int ZergSimulator::numberOfUnits(EntityType type) {
	//TODO
	//EntityData& entityData = entityDataMap.at(unitname);
	int count = 0;
	
	for (auto& x : units) {
		if ((x.getEntityData())->type == type) {
			++count;
		}
	}
	for (auto& x : upgradeableUnits) {
		if ((x.getEntityData())->type == type) {
			++count;
		}
	}
	for (auto& x : drones) {
		if ((x.getEntityData())->type == type) {
			++count;
		}
	}
	for (auto& x : queens) {
		if ((x.getEntityData())->type == type) {
			++count;
		}
	}
	
	if (count > 0) {
		return count;
	}
	
	for (auto& x : buildings) {
		if ((x.getEntityData())->type == type) {
			++count;
		}
	}
	for (auto& x : hatcheries) {
		if ((x.getEntityData())->type == type) {
			++count;
		}
	}
	for (auto& x : spires) {
		if ((x.getEntityData())->type == type) {
			++count;
		}
	}
	for (auto& x : nydusNetworks) {
		if ((x.getEntityData())->type == type) {
			++count;
		}
	}
	
	return count;
}

int ZergSimulator::numberOfWorkers() {
	return drones.size();
}

int ZergSimulator::numberOfProductionStructures() {
	//TODO
	return 0;
}






