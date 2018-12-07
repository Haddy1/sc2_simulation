#pragma once

#include "Unit.h"

class ZergUnit : public Unit {
	
public:
	ZergUnit(string);
	void morphTo(string);
};


class ZergDrone : public ZergUnit {
	bool working;
public:
	ZergDrone(string);
	void setWorking(bool b) { working = b; }
};
