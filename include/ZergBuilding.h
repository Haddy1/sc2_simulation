#pragma once

#include "Building.h"
#include "ZergUnit.h"

class ZergBuilding : public Building {
public:
	ZergBuilding(string);
	void update();
};

class ZergHatchery : public ZergBuilding {//represents hatchery, lair, hive
	int larvas;
	int larvaProgress;
	bool spawningQueen;
	int queenProgress;
	bool injectingLarvas;
	int injectProgress;
	bool upgrading;
	int upgradeProgress;
public:
	ZergHatchery(string);
	void update();
	bool upgrade();
	int getLarvaCount() const;
	bool takeLarva();
	bool spawnQueen();
	bool takeQueen();
	bool injectLarvas();
};

class ZergSpire : public ZergBuilding {//represents spire, greater_spire
	bool upgrading;
	int upgradeProgress;
public:
	ZergSpire(string);
	bool upgrade();
	void update();
};

class ZergNydusNetwork : public ZergBuilding {//represents nydus_network
	bool spawningUnit;
	int spawnProgress;
public:
	ZergNydusNetwork(string);
	void update();
	bool spawn();
	bool takeUnit();
};
