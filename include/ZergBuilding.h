#pragma once

#include "Building.h"
#include "ZergUnit.h"

class ZergBuilding : public Building {
	//bool upgrading;
	//int upgradeProgress;
public:
	ZergBuilding(string);
	//void upgrade();
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
	void upgrade();
	int getLarvaCount() const;
	void takeLarva();
	bool spawnQueen();
	ZergQueen* getQueen();
	void injectLarvas();
};

class ZergSpire : public ZergBuilding {//represents spire, greater_spire
	bool upgrading;
	int upgradeProgress;
public:
	ZergSpire(string);
	void upgrade();
	void update();
};

class ZergNydusNetwork : public ZergBuilding {//represents nydus_network
	bool spawningUnit;
	int spawnProgress;
public:
	ZergNydusNetwork(string);
	void update();
	ZergUnit* getUnit();
};
