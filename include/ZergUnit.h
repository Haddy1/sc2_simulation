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
	ZergUnit(int& ID_Counter, string, ResourceManager&);
};


class ZergLarva : public ZergUnit {
	EntityData *morphingToData;
	int morphProgress;
	int& busyCounter;
public:
	ZergLarva(int& ID_Counter, string name, ResourceManager& r, string morphingTo, int&);
	ZergLarva(int& ID_Counter, string name, ResourceManager& r, EntityData *morphingTo, int&);
	//~ZergLarva();
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
	Tech& tech;
public:
	ZergDrone(int& ID_Counter, string, ResourceManager&, Tech&, int&);
	//~ZergDrone();
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
	FixedPoint maxEnergy;
	int& busyCounter;
public:
	ZergQueen(int& ID_Counter, string, ResourceManager&, int&);
	void update();
	bool canInjectLarvas();
	bool injectLarvas();
	bool busy();
	
	static FixedPoint chargeRate;
	static FixedPoint fixed25;
};


class ZergUpgradeableUnit : public ZergUnit {
	EntityData *upgradeData;
	bool upgrading;
	int upgradeProgress;
	int& busyCounter;
	Tech& tech;
public:
	ZergUpgradeableUnit(int& ID_Counter, string, ResourceManager&, Tech&, int&);
	bool update(); //returns true if upgrade done in this step
	bool upgrade();
	bool busy();
};


