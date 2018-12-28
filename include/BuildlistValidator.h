#pragma once

#include "Race.h"
#include "EntityData.h"

#include <set>
#include <string>
#include <queue>
#include <iostream>

using std::set;
using std::string;
using std::queue;

class BuildlistValidator {
	set<string> builtTech;
	queue<string> buildQueue;
	Race race;
	
	bool gasBuildingBuilt;
	
	float supply;
	float supplyMax;
	
	bool dependencyFulfilled(EntityData& e);
public:
	BuildlistValidator(Race race, queue<string> buildQueue);
	bool validate();
	
};



