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
	
	queue<string> buildOrder;
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
	ForwardSimulator(queue<string> q) : buildOrder(q) {}
	virtual ~ForwardSimulator() = 0;
	virtual void init() = 0;
	virtual void simulate() = 0;
	
	virtual int numberOfUnits(string unitname) {return 0;}
	virtual int numberOfWorkers() {return 0;}
	virtual int numberOfProductionStructures() {return 0;}
};

inline ForwardSimulator::~ForwardSimulator() {}

class TerranSimulator : public ForwardSimulator {
	queue<string> buildOrder;
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
	TerranSimulator(queue<string>);
	~TerranSimulator() {}
	void init();
	void simulate();
};

typedef shared_ptr<ProtossBuilding> building_ptr;
typedef shared_ptr<ProtossUnit> unit_ptr;

class ProtossSimulator : public ForwardSimulator {
	// buildings
	multimap<string, building_ptr> buildings;
	vector<building_ptr> unfinishedBuildings;
	shared_ptr<Nexus> nexus;
	shared_ptr<ProtossBuilding> boosted_building;
	// units
	vector<unit_ptr> units;
	vector<unit_ptr> unfinishedUnits;
	// other members
	queue<string> buildOrder;
	Tech tech;
	ResourceManager resourceManager;
	JsonLogger logger;
	int timestep;
	int chronoboostTimer;
	int numEntities;
	int numWorkers;
public:
	ProtossSimulator(bool);
	ProtossSimulator(queue<string>, bool);
	~ProtossSimulator();
	void init();
	void simulate();
	void update_buildProgress(vector<shared_ptr<EventEntry>>&);
	void handle_chronoboost(vector<shared_ptr<EventEntry>>&);
	void process_buildlist(vector<shared_ptr<EventEntry>>&);
};


class ZergSimulator : public ForwardSimulator {
	JsonLoggerV2 logger;
	ResourceManager resourceManager;
	Tech tech;
	queue<string> buildOrder;
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
	ZergSimulator(queue<string>, bool logging, int maxTime = 1000);
	~ZergSimulator();
	void init();
	void simulate();
	int numberOfUnits(string unitname);
	int numberOfWorkers();
	int numberOfProductionStructures();
	
	int timestep;
	bool timedOut;
};


