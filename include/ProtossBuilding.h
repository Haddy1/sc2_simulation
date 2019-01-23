#pragma once
#ifndef PBUILD_H
#define PBUILD_H

#include "Building.h"
#include "FixedPoint.h"
#include "ResourceManager.h"
#include "ProtossUnit.h"
#include "Tech.h"
#include <memory>
#include <iostream>
#include <cmath>

class ProtossUnit;

using std::shared_ptr;
typedef shared_ptr<ProtossUnit> u_ptr;

class ProtossBuilding : public Building {
public:
	ProtossBuilding(int&, string, ResourceManager&, Tech&);
	bool update();
	bool isBusy();
	bool produceUnit(u_ptr);
	u_ptr getProducedUnit();
	void finishUnit();
protected:
	bool busy;
	ResourceManager& rm;
	Tech& tech;
	int curTime;
	u_ptr toProduce;
};

class Nexus : public ProtossBuilding {
public:
	Nexus(int&, string, ResourceManager&, Tech&);
	void updateEnergy();
	void consumeEnergy();
	FixedPoint getEnergy();
private:
	FixedPoint energy;
};

#endif
