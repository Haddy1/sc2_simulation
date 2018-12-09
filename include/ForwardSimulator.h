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
	ProtossSimulator();
	ProtossSimulator(queue<string>);
	~ProtossSimulator() {}
	
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
	~ZergSimulator() {}
	void simulate();
};


