#pragma once
#include "Unit.h"
#include "ResourceManager.h"
#include "ProtossBuilding.h"
#include <memory>

using std::shared_ptr;

class ProtossUnit : public Unit {
public:
	ProtossUnit(int&, string, shared_ptr<ProtossBuilding>);
	virtual bool update();
	shared_ptr<ProtossBuilding> getProducer();
private:
	int curTime;
	shared_ptr<ProtossBuilding> producer;
};