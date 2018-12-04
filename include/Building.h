#pragma once

#include "Ability.h"
#include <string>

using std::string;

class Building {
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
	bool busy();
	bool ability();
	bool createUnit(string);
};
