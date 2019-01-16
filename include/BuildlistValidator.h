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
	queue<EntityType> buildQueue;
	Race race;
	
	int gasBuildings;
	
	float supply;
	float supplyMax;
	
	void init();
	
public:
	BuildlistValidator(Race race);
	BuildlistValidator(Race race, queue<EntityType> buildQueue);
	bool validate(); // validates whole given list
	bool validateNext(EntityType t); // validates next
	bool checkNext(EntityType t); // validates next but does not change state of validator
};



