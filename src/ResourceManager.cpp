#include "../include/ResourceManager.h"

#include <cassert>

const FixedPoint ResourceManager::mineralsPerWorkerSecond(0.7);
const FixedPoint ResourceManager::vespenePerWorkerSecond(0.35);

ResourceManager::ResourceManager() : minerals(50), vespene(0), supply(0), supplyMax(0), mineralWorkers(0), vespeneWorkers(0) {
	
}

ResourceManager::~ResourceManager() {
	
}

void ResourceManager::update() {
	//TODO: error
	minerals += FixedPoint(mineralWorkers) * mineralsPerWorkerSecond;
	vespene += FixedPoint(vespeneWorkers) * vespenePerWorkerSecond;
}

bool ResourceManager::canBuild(EntityData& e) {
	return ((e.minerals <= minerals.toInt()) && (e.vespene <= vespene.toInt()) && ((e.supplyCost + supply) <= supplyMax));
}

int ResourceManager::getMinerals() {
	return minerals.toInt();
}

int ResourceManager::getVespene() {
	return vespene.toInt();
}

double ResourceManager::getSupply() {
	return supply;
}

double ResourceManager::getSupplyMax() {
	return supplyMax;
}

bool ResourceManager::consumeRes(EntityData& e) {
	if (canBuild(e)) {
		minerals -= FixedPoint(e.minerals);
		vespene -= FixedPoint(e.vespene);
		supply += e.supplyCost;
		std::cout << "paid res for " << e.name << std::endl;
		return true;
	} else {
		return false;
	}
}

bool ResourceManager::consumeMinerals(FixedPoint a) {
	if (a <= minerals) {
		minerals -= a;
		return true;
	} else {
		return false;
	}
}

bool ResourceManager::consumeVespene(FixedPoint a) {
	if (a <= vespene) {
		vespene -= a;
		return true;
	} else {
		return false;
	}
}

bool ResourceManager::consumeSupply(double a) {
	if (supply + a <= supplyMax) {
		supply += a;
		return true;
	} else {
		return false;
	}
}

bool ResourceManager::consumeMinerals(int a) {
	return consumeMinerals(FixedPoint(a));
}

bool ResourceManager::consumeVespene(int a) {
	return consumeVespene(FixedPoint(a));
}

bool ResourceManager::consumeSupply(int a) {
	return consumeSupply(static_cast<double>(a));
}

void ResourceManager::addSupplyMax(double a) {
	supplyMax += a;
}

void ResourceManager::addSupplyMax(int a) {
	supplyMax += static_cast<double>(a);
}

