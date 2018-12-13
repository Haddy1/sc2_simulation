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
	int larvaProgress;
	bool spawningQueen;
	int queenProgress;
	bool injectingLarvas;
	int injectProgress;
	bool upgrading;
	int upgradeProgress;
public:
	ZergHatchery(string, ResourceManager&);
	void update();
	bool upgrade();
	
	int getLarvaCount() const;
	bool takeLarva(); //dont use
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
public:
	ZergSpire(string, ResourceManager&);
	bool upgrade();
	void update();
	bool busy();
};


class ZergNydusNetwork : public ZergBuilding {//represents nydus_network
	EntityData& nydusWormData;
	bool spawningUnit;
	int spawnProgress;
public:
	ZergNydusNetwork(string, ResourceManager&);
	void update();
	bool spawn();
	bool takeUnit();
	bool busy();
};
