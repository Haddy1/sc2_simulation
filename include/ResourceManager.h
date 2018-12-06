#pragma once

#include "FixedPoint.h"

class ResourceManager {
	FixedPoint minerals;
	FixedPoint vespene;
	int supply;
	
	int mineralWorkers;
	int vespeneWorkers;
	
public:
	static const FixedPoint mineralsPerWorkerSecond;
	static const FixedPoint vespenePerWorkerSecond;
	
	ResourceManager();
	~ResourceManager();
	void update();
	int getMinerals();
	int getVespene();
	int getSupply();
	void consumeMinerals(FixedPoint);
	void consumeVespene(FixedPoint);
	void consumeMinerals(int);
	void consumeVespene(int);
	void consumeSupply(int);
};
