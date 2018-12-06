#pragma once

#include "Entity.h"
#include "Ability.h"
#include "Work.h"
#include <string>

using std::string;

class Unit : public Entity {
	bool working;
	string unitType;
	int energy;
	
public:
	Unit() = delete;
	Unit(string);
	~Unit();
	bool busy();
	bool assign();
	bool ability();
};
