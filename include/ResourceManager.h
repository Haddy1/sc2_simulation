#pragma once

#include "FixedPoint.h"

#include <iostream>

class ResourceManager {
	FixedPoint minerals;
	FixedPoint vespene;
	double supply;
	double supplyMax;
	
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
	double getSupply();
	double getSupplyMax();
	void consumeMinerals(FixedPoint);
	void consumeVespene(FixedPoint);
	void consumeSupply(double);
	void consumeMinerals(int);
	void consumeVespene(int);
	void consumeSupply(int);
	void addSupplyMax(double);
	void addSupplyMax(int);
	int getMineralWorkers() const { return mineralWorkers; }
	int getVespeneWorkers() const { return vespeneWorkers; }
	int getWorkers() const { return mineralWorkers + vespeneWorkers; }
	void setMineralWorkers(int a) { mineralWorkers = a; }
	void setVespeneWorkers(int a) { vespeneWorkers = a; }
	
	friend std::ostream& operator<<(std::ostream& os, const ResourceManager& r) {
		os << "Resources: " << r.minerals << ", " << r.vespene << ", " << r.supply;
		return os;
	}
};
