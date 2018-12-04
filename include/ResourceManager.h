#pragma once

class ResourceManager {
	int minerals;//todo use fixed point
	int vespene;
	int supply;
	
public:
	ResourceManager();
	~ResourceManager();
	void update();
	int getMinerals();
	int getVespene();
	int getSupply();
	void consumeMinerals(int);
	void consumeVespene(int);
	void consumeSupply(int);
};
