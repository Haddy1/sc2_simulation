#pragma once

#include "Entity.h"
#include "Ability.h"
#include <string>

using std::string;

/*
Fügt die auskommentierten variablen und methoden bei Bedarf in eure Unterklassen hinzu, da jeder andere braucht und es sonst unübersichtlich wird
*/
class Building : public Entity {
	//bool constructing;
	//int constrProgress;
	//int energy;
	//bool working;
	//int workProgress;
protected:
	//string name;
public:
	Building() = delete;
	Building(int& ID_Counter, EntityType);
	//virtual void update();
	//virtual bool busy();
	//virtual bool ability();
	//virtual bool createUnit(string);
	//string getName() { return name; }
};
