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
	EntityData& morphingToData;
	int morphProgress;
public:
	ZergLarva(string name, ResourceManager& r, string morphingTo);
	ZergLarva(string name, ResourceManager& r, EntityData& morphingTo);
	void update();
	bool isDone();
	EntityData& getUnitData();
	bool busy();
};


class ZergDrone : public ZergUnit {
	bool working;
	bool morphing;
	int morphProgress;
	EntityData* morphingToData;
public:
	ZergDrone(string, ResourceManager&);
	void setWorking(bool);
	void update();
	bool morph(string);
	bool morph(EntityData&);
	bool isMorphing();
	bool morphingDone();
	EntityData& getBuildingData();
	bool busy();
};


class ZergQueen : public ZergUnit {
	FixedPoint energy;
public:
	ZergQueen(string, ResourceManager&);
	void update();
	bool canInjectLarvas();
	bool injectLarvas();
	bool busy();
};


class ZergUpgradeableUnit : public ZergUnit {
	EntityData* upgradeData;
	bool upgrading;
	int upgradeProgress;
public:
	ZergUpgradeableUnit(string, ResourceManager&);
	void update();
	bool upgrade();
	bool busy();
};


