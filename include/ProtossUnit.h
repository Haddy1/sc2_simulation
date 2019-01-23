#pragma once
#ifndef PUNIT_H
#define PUNIT_H
#include "Unit.h"
#include "ProtossBuilding.h"
#include "ResourceManager.h"
#include "EntityData.h"
#include <memory>

class ProtossBuilding;

using std::shared_ptr;
typedef shared_ptr<ProtossBuilding> b_ptr;

class ProtossUnit : public Unit {
public:
	ProtossUnit(int&, EntityType, b_ptr, ResourceManager&);
	bool update(bool finish = true);
	b_ptr getProducer();
private:
	int curTime;
	b_ptr producer;
	ResourceManager& rm;
};

#endif
