#pragma once

#include "Entity.h"
#include "Ability.h"
#include <string>

using std::string;

class Building : public Entity {
	bool constructing;
	int constrProgress;
	string buildingType;
	int energy;
	bool working;
	int workProgress;
	
public:
	Building() = delete;
	Building(string);
	~Building();
	void update();
	bool busy();
	bool ability();
	bool createUnit(string);
};
