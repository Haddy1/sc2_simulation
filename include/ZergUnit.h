#pragma once

#include "Unit.h"
#include "ZergBuilding.h"
#include "ResourceManager.h"
#include "EntityData.h"
#include "FixedPoint.h"
#include "Tech.h"

class ZergUnit : public Unit {
protected:
	ResourceManager& r;
public:
	ZergUnit(string, ResourceManager&);
};


class ZergLarva : public ZergUnit {
	EntityData *morphingToData;
	int morphProgress;
	int& busyCounter;
public:
	ZergLarva(string name, ResourceManager& r, string morphingTo, int&);
	ZergLarva(string name, ResourceManager& r, EntityData *morphingTo, int&);
	~ZergLarva();
	void update();
	bool isDone();
	EntityData *getUnitData();
	bool busy();
};


class ZergDrone : public ZergUnit {
	//bool working;
	bool morphing;
	int morphProgress;
	EntityData *morphingToData;
	int& busyCounter;
public:
	ZergDrone(string, ResourceManager&, int&);
	~ZergDrone();
	//void setWorking(bool);
	void update();
	bool morph(string);
	bool morph(EntityData*);
	bool isMorphing();
	bool morphingDone();
	EntityData *getBuildingData();
	bool busy();
};


class ZergQueen : public ZergUnit {
	FixedPoint energy;
	int& busyCounter;
public:
	ZergQueen(string, ResourceManager&, int&);
	void update();
	bool canInjectLarvas();
	bool injectLarvas();
	bool busy();
};


class ZergUpgradeableUnit : public ZergUnit {
	EntityData *upgradeData;
	bool upgrading;
	int upgradeProgress;
	int& busyCounter;
public:
	ZergUpgradeableUnit(string, ResourceManager&, int&);
	bool update(); //returns true if upgrade done in this step
	bool upgrade();
	bool busy();
};


