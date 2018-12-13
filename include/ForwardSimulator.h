#pragma once

#include "Race.h"
#include "Building.h"
#include "Unit.h"
#include "ZergBuilding.h"
#include "ZergUnit.h"
#include "ResourceManager.h"
#include "JsonLogger.h"

#include <vector>
#include <queue>
#include <string>

using std::vector;
using std::queue;
using std::string;

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
	virtual void init() = 0;
	
public:
	ForwardSimulator() {}
	ForwardSimulator(queue<string> q) : buildOrder(q) {}
	virtual ~ForwardSimulator() = 0;
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
	
	void init();
public:
	TerranSimulator();
	TerranSimulator(queue<string>);
	~TerranSimulator() {}
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
	
	void init();
public:
	ProtossSimulator(bool);
	ProtossSimulator(queue<string>, bool);
	~ProtossSimulator() {}
	
	void simulate();
};


class ZergSimulator : public ForwardSimulator {
	ResourceManager resourceManager;
	queue<string> buildOrder;
	int timestep;
	int maxTime;
	int gasBuildings;
	
	vector<ZergBuilding*> buildings;
	vector<ZergHatchery*> hatcheries;
	vector<ZergSpire*> spires;
	vector<ZergNydusNetwork*> nydusNetworks;
	
	vector<ZergUnit*> units;
	vector<ZergUpgradeableUnit*> upgradeableUnits;
	vector<ZergDrone*> drones;
	vector<ZergLarva*> larvas;
	vector<ZergQueen*> queens;
	
	void init();
public:
	ZergSimulator();
	ZergSimulator(queue<string>);
	~ZergSimulator() {}
	void simulate();
};


