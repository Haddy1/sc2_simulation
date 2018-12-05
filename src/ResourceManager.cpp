#include "../include/ResourceManager.h"

const FixedPoint ResourceManager::mineralsPerWorkerSecond(70);
const FixedPoint ResourceManager::vespenePerWorkerSecond(35);

ResourceManager::ResourceManager() : minerals(0), vespene(0), supply(0), mineralWorkers(0), vespeneWorkers(0) {
	
}

ResourceManager::~ResourceManager() {
	
}

void ResourceManager::update() {
	//todo
}

int ResourceManager::getMinerals() {
	return minerals.toInt();
}

int ResourceManager::getVespene() {
	return vespene.toInt();
}

int ResourceManager::getSupply() {
	return supply;
}

void ResourceManager::consumeMinerals(int a) {
	//minerals -= FixedPoint(a);
	//todo
}

void ResourceManager::consumeVespene(int a) {
	//vespene -= FixedPoint(a);
	//todo
}

void ResourceManager::consumeSupply(int a) {
	//supply -= a;
	//todo
}

