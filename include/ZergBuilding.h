#pragma once

#include "Building.h"
#include "ZergUnit.h"

class ZergBuilding : public Building {
	bool upgrading;
	int upgradeProgress;
public:
	void upgrade();
};

class ZergHatchery : public ZergBuilding {//represents hatchery, lair, hive
	int larvas;
	int larvaProgress;
	bool spawningQueen;
	int queenProgress;
public:
	ZergHatchery();
	int getLarvaCount();
	void takeLarva();
	void spawnQueen();
	ZergUnit getQueen();
};


