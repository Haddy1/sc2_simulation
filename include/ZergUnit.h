#pragma once

#include "Unit.h"

class ZergUnit : public Unit {
	
public:
	void morphTo(string);
};


class ZergDrone : public ZergUnit {
	bool working;
public:
	void setWorking(bool b) { working = b; }
}
