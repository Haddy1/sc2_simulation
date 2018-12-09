#pragma once

#include "Unit.h"
#include "ZergBuilding.h"
#include "ResourceManager.h"
#include "EntityData.h"

class ZergUnit : public Unit {
protected:
	ResourceManager& r;
public:
	ZergUnit(string, ResourceManager&);
};


class ZergLarva : public ZergUnit {
public:
	ZergLarva(string, ResourceManager&);
};


class ZergDrone : public ZergUnit {
	bool working;
	bool morphing;
	int morphProgress;
	string morphingTo;
public:
	ZergDrone(string, ResourceManager&);
	void setWorking(bool);
	bool morph(string);
	//ZergBuilding* getMorphedBuilding();
};


class ZergQueen : public ZergUnit {
int energy;
public:
	ZergQueen(string, ResourceManager&);
	void update();
	bool injectLarva();
};
