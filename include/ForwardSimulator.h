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
	ResourceManager resourceManager;
	int timestep;
	bool running;
public:
	ForwardSimulator(Race);
	ForwardSimulator(Race, queue<string>);
	void simulate();
};


