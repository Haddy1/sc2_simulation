#include "../include/ResourceManager.h"

#include <cassert>

const FixedPoint ResourceManager::mineralsPerWorkerSecond(0.7);
const FixedPoint ResourceManager::vespenePerWorkerSecond(0.35);

ResourceManager::ResourceManager() : minerals(50), vespene(0), supply(0), mineralWorkers(0), vespeneWorkers(0) {
	
}

ResourceManager::~ResourceManager() {
	
}

void ResourceManager::update() {
	//TODO: error
	minerals += FixedPoint(mineralWorkers) * mineralsPerWorkerSecond;
	vespene += FixedPoint(vespeneWorkers) * vespenePerWorkerSecond;
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

void ResourceManager::consumeMinerals(FixedPoint a) {
	assert(a <= minerals);
	minerals -= a;
}

void ResourceManager::consumeVespene(FixedPoint a) {
	assert(a <= vespene);
	vespene -= a;
}

void ResourceManager::consumeMinerals(int a) {
	consumeMinerals(FixedPoint(a));
}

void ResourceManager::consumeVespene(int a) {
	consumeVespene(FixedPoint(a));
}

void ResourceManager::consumeSupply(int a) {
	assert(a <= supply);
	supply -= a;
}

