#include "ForwardSimulator.h"

typedef multimap<EntityType, building_ptr>::iterator MIter;

// param. constructors
ProtossSimulator::ProtossSimulator(bool validBuildlist, bool log, int mt) : 
	logger(PROTOSS, resourceManager, validBuildlist), logging(log), \
	timestep(1), maxTime(mt), chronoboostTimer(-1), numEntities(0), timeout(false) {
	// ...
}

ProtossSimulator::ProtossSimulator(queue<EntityType> q, bool validBuildlist, bool log, int mt) : 
	buildOrder(q), logger(PROTOSS, resourceManager, validBuildlist), logging(log), \
	timestep(1), maxTime(mt), chronoboostTimer(-1), numEntities(0), timeout(false) {
	// ...
}

// destructor
ProtossSimulator::~ProtossSimulator() {
	
}

void ProtossSimulator::init() {
	// setup: 1 Nexus
	shared_ptr<Nexus> nex = make_shared<Nexus>(Nexus(numEntities, nexus, resourceManager, tech));
	nexuses.push_back(nex);
	buildings.emplace(nexus, nex);
	resourceManager.addSupplyMax(10);
	tech.add(nexus);
	
	// setup: 6 probes
	for(int i = 0; i < 6; ++i) {
		unit_ptr u = make_shared<ProtossUnit>(ProtossUnit(numEntities, probe, nex, resourceManager));
		units.emplace(make_pair(u->getType(), numberOfUnits(u->getType())+1));
	}
	resourceManager.setMineralWorkers(6);
	resourceManager.consumeSupply(6);
	
	// log setup
	vector<pair<string, vector<int>>> initUnits = {
		make_pair<string, vector<int>>("nexus", {0}),
		make_pair<string, vector<int>>("probe", {1, 2, 3, 4, 5, 6})
	};
	if(logging) {
		logger.printSetup(initUnits);
		logger.printMessageStart();
	}
}

void ProtossSimulator::handle_chronoboost(vector<shared_ptr<EventEntry>>& events) {
	// update current energy
	for(shared_ptr<Nexus> n : nexuses) {
		n->updateEnergy();
	}
	// select building to boost if possible
	for(shared_ptr<Nexus> n : nexuses) {
		if(n->getEnergy() >= FixedPoint(25) && chronoboostTimer == -1) {
			if(n->isBusy()) {
				boosted_building = n;
				chronoboostTimer = 0;
				n->consumeEnergy();
				events.push_back(create_ability_ptr("special", "chronoboost", boosted_building->getID(), n->getID()));
				break;
			}
		}
	}
	
	// chronoboost already active
	if(chronoboostTimer >= 0) {
		unit_ptr u = boosted_building->getProducedUnit();
		if(boosted_building != nullptr && u != nullptr) {
			u->update(0.5, false);
			/*
				events.push_back(create_event_ptr("build-end", u->getName(), boosted_building->getID(), u->getID()));
				units.emplace(make_pair(u->getType(), numberOfUnits(u->getType())+1));
				unfinishedUnits.erase(std::remove(unfinishedUnits.begin(), unfinishedUnits.end(), u), unfinishedUnits.end()); // remove unit
			}
			*/
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
			if((*i)->getType() == nexus) {
				int dummy = 0; // prevent from getting new id
				shared_ptr<Nexus> n = make_shared<Nexus>(dummy, nexus, resourceManager, tech);
				n->setID((*i)->getID());
				nexuses.push_back(n);
			}
			buildings.emplace((*i)->getType(), *i);
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
			units.emplace(make_pair((*i)->getType(), numberOfUnits((*i)->getType())+1));
			unfinishedUnits.erase(i);
		} else {
			++i;
		}
	}
}

void ProtossSimulator::process_buildlist(vector<shared_ptr<EventEntry>>& events) {
	if(!buildOrder.empty() && chronoboostTimer != 1) {
		EntityType type = buildOrder.front();
		EntityData e = entityDataMap.at(type);
		
		if(timestep + e.buildTime >= maxTime) {
			buildOrder.pop();
			return;
		}
		
		if(resourceManager.canBuild(e) && tech.dependencyFulfilled(e)) {
			bool beginProduction = false;
			int producerID = -1;
			
			if(e.isBuilding) {
				beginProduction = true;
				producerID = 1; // always warped by same probe since it is never occupied while building (maybe TODO)
				unfinishedBuildings.push_back(make_shared<ProtossBuilding>(ProtossBuilding(numEntities, type, resourceManager, tech)));
			} else {
				// find available producer building
				pair<MIter, MIter> mapIter = buildings.equal_range(e.producedBy.front());
				for(auto i = mapIter.first; i != mapIter.second;) {
					building_ptr building = (*i).second;
					if(!building->isBusy()) {
						unit_ptr u = make_shared<ProtossUnit>(ProtossUnit(numEntities, type, building, resourceManager));
						beginProduction = building->produceUnit(u);
						unfinishedUnits.push_back(u);
						producerID = building->getID();
						// perform update on new unit if chronoboost already active
						if(chronoboostTimer > 0 && producerID == boosted_building->getID())
							u->update(0.5);
						break;
					} else {
						++i;
					}
				}
			}
			// entity construction has begun -> log and remove from buildlist 
			if(beginProduction) {
				resourceManager.consumeRes(e);
				events.push_back(create_event_ptr("build-start", e.name, producerID));
				buildOrder.pop();
			}
		}
	}	
}

int ProtossSimulator::numberOfUnits(EntityType name) {
	map<EntityType, int>::iterator it;
	it = units.find(name);
	if(it != units.end()) {
		return it->second;
	} else {
		int c = 0;
		for(auto& b : buildings) {
			if(b.first == name) {
				++c;
			}
		}
		return c;
	}
	return 0;
}

void ProtossSimulator::simulate() {
	while(timestep <= maxTime) {
		vector<shared_ptr<EventEntry>> vec;
		resourceManager.update();
		
		update_buildProgress(vec);
		
		handle_chronoboost(vec);
		
		process_buildlist(vec);
		
		resourceManager.redistributeWorkers();
		
		if(logging) {
			logger.printMessage(timestep, vec);
		}
		
		if(buildOrder.empty() && unfinishedBuildings.empty() && unfinishedUnits.empty()) {
			return;
		}
		++timestep;
	}
	
	if(timestep > maxTime) {
		timeout = true;
	}
}
