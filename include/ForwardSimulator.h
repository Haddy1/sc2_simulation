#pragma once

#include "Race.h"
#include "Building.h"
#include "Unit.h"
#include "ZergBuilding.h"
#include "ZergUnit.h"
#include "ResourceManager.h"
#include "JsonLogger.h"
#include "JsonLoggerV2.h"
#include "EntityData.h"
#include "ProtossBuilding.h"
#include "ProtossUnit.h"

#include <algorithm>
#include <vector>
#include <queue>
#include <string>
#include <list>
#include <memory>
#include <map>

using std::vector;
using std::queue;
using std::string;
using std::stoi;
using std::list;
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;
using std::begin;
using std::end;
using std::map;
using std::multimap;
using std::pair;


class ForwardSimulator {
	
	queue<EntityType> buildOrder;
	/*
	vector<Building> buildings;
	vector<Unit> units;
	vector<Unit> workers;
	ResourceManager resourceManager;
	int timestep;
	bool running;
	*/
	
public:
	ForwardSimulator() {}
	ForwardSimulator(queue<EntityType> q) : buildOrder(q) {}
	virtual ~ForwardSimulator() = 0;
	virtual void init() = 0;
	virtual void simulate() = 0;
	
	virtual int numberOfUnits(EntityType unitname) {return 0;}
	virtual int numberOfWorkers() {return 0;}
	virtual int numberOfProductionStructures() {return 0;}
};

inline ForwardSimulator::~ForwardSimulator() {}




/*
class TerranSimulator : public ForwardSimulator {
	queue<EntityType> buildOrder;
    vector<EventEntry> eventList_;
	ResourceManager rm;
	Tech tech;
	int maxTime = 1000;
	int timestep;
	bool running;
	JsonLoggerV2 logger;
    int ID_Counter;
	
public:
	TerranSimulator();
	TerranSimulator(queue<EntityType>);
	~TerranSimulator() {}
	void init();
	void simulate();
};
*/

typedef shared_ptr<ProtossBuilding> building_ptr;
typedef shared_ptr<ProtossUnit> unit_ptr;

class ProtossSimulator : public ForwardSimulator {
	// buildings
	multimap<EntityType, building_ptr> buildings;
	vector<building_ptr> unfinishedBuildings;
	vector<shared_ptr<Nexus>> nexuses;
	shared_ptr<Nexus> boosted_building;
	// units
	map<EntityType, int> units;
	vector<unit_ptr> unfinishedUnits;
	// other members
	queue<EntityType> buildOrder;
	Tech tech;
	ResourceManager resourceManager;
	JsonLogger logger;
	bool logging;
	int timestep;
	int maxTime;
	int chronoboostTimer;
	int numEntities;
	int numWorkers;
	bool timeout;
public:
	ProtossSimulator(bool, bool, int);
	ProtossSimulator(queue<EntityType>, bool, bool, int);
	~ProtossSimulator();
	void init();
	void simulate();
	void update_buildProgress(vector<shared_ptr<EventEntry>>&);
	void handle_chronoboost(vector<shared_ptr<EventEntry>>&);
	void process_buildlist(vector<shared_ptr<EventEntry>>&);
	int getTimesteps() {return timestep;};
	bool timedOut() {return timeout;};
	int numberOfUnits(EntityType);
	int numberOfWorkers() {return resourceManager.getWorkers();}
	int numberOfProductionStructures() {return buildings.size();}
};




class ZergSimulator : public ForwardSimulator {
	JsonLoggerV2 logger;
	ResourceManager resourceManager;
	Tech tech;
	queue<EntityType> buildOrder;
	int maxTime;
	int gasBuildings;
	int busyCounter;
	int ID_Counter;
	bool logging;
	
	list<ZergBuilding> buildings;
	list<ZergHatchery> hatcheries;
	list<ZergSpire> spires;
	list<ZergNydusNetwork> nydusNetworks;
	
	list<ZergUnit> units;
	list<ZergUpgradeableUnit> upgradeableUnits;
	list<ZergDrone> drones;
	list<ZergLarva> larvas;
	list<ZergQueen> queens;
	
public:
	ZergSimulator(bool logging, int maxTime = 1000);
	ZergSimulator(queue<EntityType>, bool logging, int maxTime = 1000);
	~ZergSimulator();
	void init();
	void simulate();
	int numberOfUnits(EntityType type);
	int numberOfWorkers();
	int numberOfProductionStructures();
	
	int timestep;
	bool timedOut;
};


