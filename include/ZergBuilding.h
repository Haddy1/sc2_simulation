#pragma once

#include "Building.h"
#include "ZergUnit.h"

class ZergBuilding : public Building {
	bool upgrading;
	int upgradeProgress;
public:
	ZergBuilding(string);
	void upgrade();
	void update();
};

class ZergHatchery : public ZergBuilding {//represents hatchery, lair, hive
	int larvas;
	int larvaProgress;
	bool spawningQueen;
	int queenProgress;
public:
	ZergHatchery(string);
	int getLarvaCount();
	void takeLarva();
	void spawnQueen();
	ZergUnit getQueen();
};


