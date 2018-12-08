#pragma once

#include "FixedPoint.h"
#include "EntityData.h"

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
	
	bool canBuild(EntityData&);
	bool consumeRes(EntityData&);
	
	int getMinerals();
	int getVespene();
	double getSupply();
	double getSupplyMax();
	int getMineralWorkers() const { return mineralWorkers; }
	int getVespeneWorkers() const { return vespeneWorkers; }
	int getWorkers() const { return mineralWorkers + vespeneWorkers; }
	
	bool consumeMinerals(FixedPoint);
	bool consumeVespene(FixedPoint);
	bool consumeSupply(double);
	bool consumeMinerals(int);
	bool consumeVespene(int);
	bool consumeSupply(int);
	
	void addSupplyMax(double);
	void addSupplyMax(int);
	
	void setMineralWorkers(int a) { mineralWorkers = a; }
	void setVespeneWorkers(int a) { vespeneWorkers = a; }
	
	friend std::ostream& operator<<(std::ostream& os, const ResourceManager& r) {
		os << "Resources: " << r.minerals << ", " << r.vespene << ", " << r.supply;
		return os;
	}
};
