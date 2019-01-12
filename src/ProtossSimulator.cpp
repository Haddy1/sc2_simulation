#include "ForwardSimulator.h"

typedef multimap<string, building_ptr>::iterator MIter;

// param. constructors
ProtossSimulator::ProtossSimulator(bool validBuildlist) : 
	logger(PROTOSS, resourceManager, validBuildlist), \
	timestep(1), chronoboostTimer(-1), numEntities(0) {
	// ...
}

ProtossSimulator::ProtossSimulator(queue<string> q, bool validBuildlist) : 
	buildOrder(q), logger(PROTOSS, resourceManager, validBuildlist), \
	timestep(1), chronoboostTimer(-1), numEntities(0) {
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
		units.push_back(make_shared<ProtossUnit>(ProtossUnit(numEntities, "probe", nexus, resourceManager)));
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
		if(nexus->isBusy()) {
			boosted_building = nexus;
			chronoboostTimer = 0;
			nexus->consumeEnergy();
			events.push_back(create_ability_ptr("special", "chronoboost", boosted_building->getID(), nexus->getID()));
		}
		/*
		for(auto i = begin(buildings); i != end(buildings); ++i) {
			if((*i).second->isBusy()) {
				boosted_building = (*i).second;
				chronoboostTimer = 0;
				nexus->consumeEnergy();
				events.push_back(create_ability_ptr("special", "chronoboost", boosted_building->getID(), nexus->getID()));
				break;
			}
		}
		*/
	}
	
	// chronoboost already active
	if(chronoboostTimer >= 0) {
		unit_ptr u = boosted_building->getProducedUnit();
		if(boosted_building != nullptr && u != nullptr) {
			if(chronoboostTimer % 2) {
				if(u->update()) { // production complete
					events.push_back(create_event_ptr("build-end", u->getName(), boosted_building->getID(), u->getID()));
					units.push_back(u);
					unfinishedUnits.erase(std::remove(unfinishedUnits.begin(), unfinishedUnits.end(), u), unfinishedUnits.end()); // remove unit
				}
			}
		}
		++chronoboostTimer;
	}
	// reset timer
	chronoboostTimer = (chronoboostTimer >= 20) ? -1 : chronoboostTimer;
}

void ProtossSimulator::update_buildProgress(vector<shared_ptr<EventEntry>>& events) {
	// Advance/finish buildings 
	for(auto i = begin(unfinishedBuildings); i != end(unfinishedBuildings);) {
		if((*i)->update()) {
			buildings.emplace((*i)->getName(), *i);
			events.push_back(create_event_ptr("build-end", (*i)->getName(), 1, (*i)->getID()));
			unfinishedBuildings.erase(i);
		} else {
			++i;
		}
	}
	
	// Advance/finish units
	for(auto i = begin(unfinishedUnits); i != end(unfinishedUnits);) {
		if((*i)->update()) {
			events.push_back(create_event_ptr("build-end", (*i)->getName(), (*i)->getProducer()->getID(), (*i)->getID()));
			units.push_back(*i);
			unfinishedUnits.erase(i);
		} else {
			++i;
		}
	}
}

void ProtossSimulator::process_buildlist(vector<shared_ptr<EventEntry>>& events) {
	if(!buildOrder.empty() && chronoboostTimer != 1) {
		string s = buildOrder.front();
		EntityData e = entityDataMap.at(s);
		if(resourceManager.canBuild(e) && dependencyFulfilled(e)) {
			bool beginProduction = false;
			int producerID = -1;
			
			if(e.isBuilding) {
				beginProduction = true;
				producerID = 1; // always warped by same probe since it is never occupied while building (maybe TODO)
				unfinishedBuildings.push_back(make_shared<ProtossBuilding>(ProtossBuilding(numEntities, s, resourceManager)));
			} else {
				// find available producer building
				pair<MIter, MIter> mapIter = buildings.equal_range(e.producedBy.front());
				for(auto i = mapIter.first; i != mapIter.second;) {
					building_ptr building = (*i).second;
					if(!building->isBusy()) {
						unit_ptr u = make_shared<ProtossUnit>(ProtossUnit(numEntities, s, building, resourceManager));
						beginProduction = building->produceUnit(u);
						unfinishedUnits.push_back(u);
						producerID = building->getID();
						break;
					} else {
						++i;
					}
				}
			}
			// entity construction has begun -> log and remove from buildlist 
			if(beginProduction) {
				resourceManager.consumeRes(e);
				events.push_back(create_event_ptr("build-start", s, producerID));
				buildOrder.pop();
			}
		}
	}	
}

void ProtossSimulator::simulate() {
	while(timestep < 1000) {
		vector<shared_ptr<EventEntry>> vec;
		resourceManager.update();
		
		update_buildProgress(vec);
		
		handle_chronoboost(vec);
		
		process_buildlist(vec);
		
		resourceManager.redistributeWorkers();
		
		logger.printMessage(timestep, vec);
		
		if(buildOrder.empty() && unfinishedBuildings.empty() && unfinishedUnits.empty()) {
			return;
		}
		++timestep;
	}
}