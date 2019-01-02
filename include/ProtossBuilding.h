#pragma once
#include "Building.h"
#include "FixedPoint.h"
#include "ResourceManager.h"
#include "Tech.h"
#include <iostream>
#include <cmath>

class ProtossBuilding : public Building {
public:
	ProtossBuilding(int&, string, ResourceManager&);
	virtual bool update();
	bool busy;
protected:
	ResourceManager& r;
	int curTime;
};

class Nexus : public ProtossBuilding {
public:
	Nexus(int&, string, ResourceManager&);
	bool update();
	FixedPoint getEnergy();
	void consumeEnergy();
private:
	FixedPoint energy;
};

class Gateway : public ProtossBuilding {
public:
	Gateway(int&, string, ResourceManager&);
};