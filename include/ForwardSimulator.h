#pragma once

#include "Race.h"
#include "Building.h"
#include "Unit.h"
#include "ResourceManager.h"

#include <vector>
#include <queue>

using std::vector;
using std::queue;

class ForwardSimulator {
	Race race;
	queue<string> buildOrder;
	vector<Building> buildings;
	vector<Unit> units;
	vector<Unit> workers;
	ResourceManager resourceManager;
	int timestep;
	bool running;
	
	void init();
	void initTerran();
	void initProtoss();
	void initZerg();
	void simulateTerran();
	void simulateProtoss();
	void simulateZerg();
public:
	ForwardSimulator(Race);
	ForwardSimulator(Race, queue<string>);
	void simulate();
};


