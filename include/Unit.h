#pragma once

#include "Entity.h"
#include "Ability.h"
#include "Work.h"
#include <string>

using std::string;

/*
Fügt die auskommentierten variablen und methoden bei Bedarf in eure Unterklassen hinzu, da jeder andere braucht und es sonst unübersichtlich wird
*/
class Unit : public Entity {
	//bool working;
	string name;
	//int energy;
	
public:
	Unit() = delete;
	Unit(string);
	~Unit();
	//bool busy();
	//bool assign();
	//bool ability();
};
