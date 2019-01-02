#pragma once

#include "FixedPoint.h"
#include "EntityData.h"

#include <iostream>

class ResourceManager {
	FixedPoint minerals;
	FixedPoint vespene;
	float supply;
	float supplyMax;
	
	int mineralWorkers;
	int vespeneWorkers;
	
public:
	static const FixedPoint mineralsPerWorkerSecond;
	static const FixedPoint vespenePerWorkerSecond;
	
	ResourceManager();
	~ResourceManager();
	
	void update();
	
	bool canBuild(const EntityData&);
	bool consumeRes(const EntityData&);
	bool canBuild(const EntityData&, int);
	bool consumeRes(const EntityData&, int);
	
	int getMinerals();
	int getVespene();
	float getSupply();
	float getSupplyMax();
	int getMineralWorkers() const { return mineralWorkers; }
	int getVespeneWorkers() const { return vespeneWorkers; }
	int getWorkers() const { return mineralWorkers + vespeneWorkers; }
	int getSupplyInt();
	int getSupplyMaxInt();
	
	bool consumeMinerals(FixedPoint);
	bool consumeVespene(FixedPoint);
	bool consumeSupply(float);
	bool consumeMinerals(int);
	bool consumeVespene(int);
	bool consumeSupply(int);
	
	void addSupplyMax(float);
	void addSupplyMax(int);
	
	void decrementSupply(); //used by zerg when a drone morphs
	void decreaseSupply(float);
	void increaseSupply(float);
	
	void setMineralWorkers(int a) { mineralWorkers = a; }
	void setVespeneWorkers(int a) { vespeneWorkers = a; }
	
	void incrementMineralWorkers() { ++mineralWorkers; }
	
	void redistributeWorkers(int geysers) {
		vespeneWorkers = std::min(getWorkers(), 3 * geysers);
		mineralWorkers = getWorkers() - vespeneWorkers;
	}
	
	friend std::ostream& operator<<(std::ostream& os, const ResourceManager& r) {
		os << "Resources: " << r.minerals << ", " << r.vespene << ", " << r.supply << "/" << r.supplyMax;
		return os;
	}
};
