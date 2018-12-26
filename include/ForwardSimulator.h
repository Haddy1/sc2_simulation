#pragma once

#include "Race.h"
#include "Building.h"
#include "Unit.h"
#include "ZergBuilding.h"
#include "ZergUnit.h"
#include "ResourceManager.h"
#include "JsonLogger.h"
#include "JsonLoggerV2.h"

#include <vector>
#include <queue>
#include <string>
#include <list>

using std::vector;
using std::queue;
using std::string;
using std::list;

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
};

inline ForwardSimulator::~ForwardSimulator() {}

class TerranSimulator : public ForwardSimulator {
	queue<string> buildOrder;
	vector<Building*> buildings;
	vector<Unit*> units;
	vector<Unit*> workers;
	ResourceManager resourceManager;
	int timestep;
	bool running;
	
public:
	TerranSimulator();
	TerranSimulator(queue<string>);
	~TerranSimulator() {}
	void init();
	void simulate();
};


class ProtossSimulator : public ForwardSimulator {
	queue<string> buildOrder;
	vector<Building*> buildings;
	vector<Unit*> units;
	vector<Unit*> workers;
	ResourceManager resourceManager;
	JsonLogger logger;
	int timestep;
	bool running;
	
public:
	ProtossSimulator(bool);
	ProtossSimulator(queue<string>, bool);
	~ProtossSimulator() {}
	void init();
	void simulate();
};


class ZergSimulator : public ForwardSimulator {
	JsonLoggerV2 logger;
	ResourceManager resourceManager;
	queue<string> buildOrder;
	int timestep;
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
	ZergSimulator(bool);
	ZergSimulator(queue<string>, bool);
	~ZergSimulator();
	void init();
	void simulate();
};


