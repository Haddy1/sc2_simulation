#pragma once

#include "Race.h"
#include "EntityData.h"
#include "Tech.h"

#include <set>
#include <string>
#include <queue>
#include <iostream>

using std::set;
using std::string;
using std::queue;

class BuildlistValidator {
	Tech tech;
	queue<string> buildQueue;
	Race race;
	
	int gasBuildings;
	
	float supply;
	float supplyMax;
	
public:
	BuildlistValidator(Race race, queue<string> buildQueue);
	bool validate();
	
};



