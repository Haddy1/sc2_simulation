#pragma once

#include "Unit.h"
#include "ZergBuilding.h"

class ZergUnit : public Unit {
public:
	ZergUnit(string);
};

class ZergLarva : public ZergUnit {
public:
	ZergLarva(string);
};

class ZergDrone : public ZergUnit {
	bool working;
	bool morphing;
	int morphProgress;
	string morphingTo;
public:
	ZergDrone(string);
	void setWorking(bool);
	bool morph(string);
	ZergBuilding* getMorphedBuilding();
};

class ZergQueen : public ZergUnit {
int energy;
public:
	ZergQueen(string);
	void update();
	bool injectLarva();
};
