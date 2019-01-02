#include "ForwardSimulator.h"

typedef multimap<string, building_ptr>::iterator MIter;

// param. constructors
ProtossSimulator::ProtossSimulator(bool validBuildlist) : 
	logger(PROTOSS, resourceManager, validBuildlist, "output/log.txt"), \
	timestep(1), running(true), chronoboostTimer(-1), numEntities(0), geysers(0) {
	// ...
}

ProtossSimulator::ProtossSimulator(queue<string> q, bool validBuildlist) : 
	buildOrder(q), logger(PROTOSS, resourceManager, validBuildlist, "output/log.txt"), \
	timestep(1), running(true), chronoboostTimer(-1), numEntities(0), geysers(0) {
	// ...
}

// destructor
ProtossSimulator::~ProtossSimulator() {
	
}

void ProtossSimulator::init() {
	// setup: 1 Nexus, 6 probes
	nexus = make_shared<Nexus>(Nexus(numEntities, "nexus", resourceManager));
	buildings.emplace("nexus", nexus);
	for(int i = 0; i < 6; ++i) {
		units.push_back(make_shared<ProtossUnit>(ProtossUnit(numEntities, "probe", nexus)));
	}
	resourceManager.setMineralWorkers(6);
	resourceManager.consumeSupply(6);
	
	// log setup
	vector<pair<string, vector<int>>> initUnits = {
		make_pair<string, vector<int>>("nexus", {0}),
		make_pair<string, vector<int>>("probe", {1, 2, 3, 4, 5, 6})
	};
	logger.printSetup(initUnits);
	logger.printMessageStart();
}

void ProtossSimulator::handle_chronoboost(vector<shared_ptr<EventEntry>>& events) {
	// update current energy
	nexus->update();
	// select building to boost if possible
	if(nexus->getEnergy() >= FixedPoint(25) && chronoboostTimer == -1) {
		for(auto i = begin(buildings); i != end(buildings); ++i) {
			if((*i).second->busy) {
				chronoboostTimer = 0;
				nexus->consumeEnergy();
				boosted_building = (*i).second;
				break;
			}
		}
	}
	// log
	if(chronoboostTimer == 0) {
		events.push_back(create_ability_ptr("special", "chronoboost", boosted_building->getID(), nexus->getID()));
	}
	// chronoboost already active
	if(chronoboostTimer >= 0) {
		if(chronoboostTimer >= 19) {
			chronoboostTimer = -1; // reset
			return;
		}
		if(boosted_building != nullptr && chronoboostTimer % 2) {
			if(boosted_building->update()) { // increase update rate by 50%
				chronoboostTimer = -1;
			}
		}
		++chronoboostTimer;
	}
}

void ProtossSimulator::update_buildProgress(vector<shared_ptr<EventEntry>>& events) {
	// Advance/finish buildings 
	for(auto i = begin(unfinishedBuildings); i != end(unfinishedBuildings);) {
		if((*i)->update()) {
			if((*i)->getName().compare("assimilator") == 0) {
				++geysers;
			}
			events.push_back(create_event_ptr("build-end", (*i)->getName(), 1, (*i)->getID()));
			buildings.emplace((*i)->getName().c_str(), *i);
			unfinishedBuildings.erase(i);
		} else {
			++i;
		}
	}
	
	// Advance/finish units
	for(auto i = begin(unfinishedUnits); i != end(unfinishedUnits);) {
		if((*i)->update()) {
			if((*i)->getName().compare("probe") == 0) {
				//nexus->busy = false;
				resourceManager.incrementMineralWorkers(); // redistribution occurs before log
			}
			(*i)->getProducer()->busy = false;
			events.push_back(create_event_ptr("build-end", (*i)->getName(), (*i)->getProducer()->getID(), (*i)->getID()));
			units.push_back(*i);
			unfinishedUnits.erase(i);
		} else {
			++i;
		}
	}
}

void ProtossSimulator::process_buildlist(vector<shared_ptr<EventEntry>>& events) {
	// TODO
	if(!buildOrder.empty() && chronoboostTimer < 0) {
		string s = buildOrder.front();
		EntityData e = entityDataMap.at(s);
		if(resourceManager.canBuild(e) && dependencyFulfilled(e)) {
			bool beginProduction = false;
			int producerID = -1;
			
			if(e.isBuilding) {
				if(s.compare("gateway") == 0) {
					gateways.push_back(make_shared<Gateway>(Gateway(numEntities, s, resourceManager)));
					beginProduction = true;
				}
				else if(s.compare("warpgate") == 0) {
					for(auto g = begin(gateways); g != end(gateways);) {
						if((*g)->isUpgradable()) {
							gateways.erase(g);
							beginProduction = true;
						} else {
							++g;
						}
					}
				} else {
					beginProduction = true;
				}
				producerID = 1; // always warped by same probe since it is never occupied while building (maybe TODO)
				resourceManager.consumeRes(e);
				unfinishedBuildings.push_back(make_shared<ProtossBuilding>(ProtossBuilding(numEntities, s, resourceManager)));
			} else {
				// find available producer building
				pair<MIter, MIter> mapIter = buildings.equal_range(e.producedBy.front());
				for(auto i = mapIter.first; i != mapIter.second;) {
					building_ptr building = (*i).second;
					if(!building->busy) {
						unfinishedUnits.push_back(make_shared<ProtossUnit>(ProtossUnit(numEntities, s, building)));
						building->busy = true;
						beginProduction = true;
						resourceManager.consumeRes(e);
						producerID = building->getID();
					} else {
						++i;
					}
				}
			}
			if(beginProduction) {
				events.push_back(create_event_ptr("build-start", s, producerID));
				buildOrder.pop();
			}
		}
	}	
}

void ProtossSimulator::simulate() {
	while(timestep < 100000) {
		vector<shared_ptr<EventEntry>> vec;
		resourceManager.update();
		
		update_buildProgress(vec);
		
		//handle_chronoboost(vec);
		
		process_buildlist(vec);
		
		resourceManager.redistributeWorkers(geysers);
		
		logger.printMessage(timestep, vec);
		
		if(buildOrder.empty() && unfinishedBuildings.empty() && unfinishedUnits.empty()) {
			return;
		}
		++timestep;
	}
}