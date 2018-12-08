#pragma once

#include "Building.h"
#include "Unit.h"
#include "ZergBuilding.h"
#include "ZergUnit.h"
#include "ResourceManager.h"

#include <vector>
#include <queue>

using std::vector;
using std::queue;

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
	queue<string> buildOrder;
	vector<ZergBuilding*> buildings;
	vector<ZergUnit*> units;
	vector<ZergUnit*> workers;
	ResourceManager resourceManager;
	int timestep;
	bool running;
	
	void init();
public:
	ZergSimulator();
	ZergSimulator(queue<string>);
	void simulate();
};


