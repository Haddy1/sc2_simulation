#pragma once

#include "Building.h"
#include "Unit.h"
#include "ZergBuilding.h"
#include "ZergUnit.h"
#include "ResourceManager.h"

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
	virtual void simulate() = 0;
};


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
	void simulate();
};


class ProtossSimulator : public ForwardSimulator {
	queue<string> buildOrder;
	vector<Building*> buildings;
	vector<Unit*> units;
	vector<Unit*> workers;
	ResourceManager resourceManager;
	int timestep;
	bool running;
	
	void init();
public:
	ProtossSimulator();
	ProtossSimulator(queue<string>);
	void simulate();
};


class ZergSimulator : public ForwardSimulator {
	ResourceManager resourceManager;
	queue<string> buildOrder;
	int timestep;
	int maxTime;
	bool running;
	
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
	void simulate();
};


