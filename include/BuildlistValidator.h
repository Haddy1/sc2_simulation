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
	BuildlistValidator(Race race);
	BuildlistValidator(Race race, queue<string> buildQueue);
	bool validate(); // validates whole given list
	bool validateNext(string s); // validates next
	bool checkNext(string s); // validates next but does not change state of validator
};



