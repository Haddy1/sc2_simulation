#pragma once
#ifndef PUNIT_H
#define PUNIT_H
#include "Unit.h"
#include "ProtossBuilding.h"
#include "ResourceManager.h"
#include "EntityData.h"
#include <memory>
#include <iostream>

class ProtossBuilding;

using std::shared_ptr;
typedef shared_ptr<ProtossBuilding> b_ptr;

class ProtossUnit : public Unit {
public:
	ProtossUnit(int&, EntityType, b_ptr, ResourceManager&);
	bool update(double rate = 1.0, bool finish = true);
	b_ptr getProducer();
private:
	double curTime;
	b_ptr producer;
	ResourceManager& rm;
};

#endif
