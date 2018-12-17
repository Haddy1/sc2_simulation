#pragma once

#include "Building.h"
#include "ZergUnit.h"
#include "ResourceManager.h"
#include "EntityData.h"
#include "Tech.h"

class ZergBuilding : public Building {
protected:
	ResourceManager& r;
public:
	ZergBuilding(string, ResourceManager&);
};


class ZergHatchery : public ZergBuilding {//represents hatchery, lair, hive
	EntityData& lairData;
	EntityData& hiveData;
	EntityData& queenData;
	int larvas;
	bool spawningLarva;
	int larvaProgress;
	bool spawningQueen;
	int queenProgress;
	bool injectingLarvas;
	int injectProgress;
	bool upgrading;
	int upgradeProgress;
	int& busyCounter;
public:
	ZergHatchery(string, ResourceManager&, int&);
	bool update(); //returns true if upgrade done in this step
	bool upgrade();
	
	int getLarvaCount() const;
	//bool takeLarva(); //dont use
	bool morphLarva(string); //can only take larva from hatchery by morphing
	bool morphLarva(EntityData&);
	
	bool spawnQueen();
	bool takeQueen();
	bool injectLarvas();
	bool busy();
};


class ZergSpire : public ZergBuilding {//represents spire, greater_spire
	EntityData& greaterSpireData;
	bool upgrading;
	int upgradeProgress;
	int& busyCounter;
public:
	ZergSpire(string, ResourceManager&, int&);
	bool upgrade();
	bool update(); //returns true if upgrade done in this step
	bool busy();
};


class ZergNydusNetwork : public ZergBuilding {//represents nydus_network
	EntityData& nydusWormData;
	bool spawningUnit;
	int spawnProgress;
	int& busyCounter;
public:
	ZergNydusNetwork(string, ResourceManager&, int&);
	void update();
	bool spawn();
	bool takeUnit();
	bool busy();
};
