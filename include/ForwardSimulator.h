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
	queue<string> buildOrder;
	vector<Building> buildings;
	vector<Unit> units;
	ResourceManager resourceManager;
	int timestep;
public:
	ForwardSimulator(Race);
	void simulate();
};


