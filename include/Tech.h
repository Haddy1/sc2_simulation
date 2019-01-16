#pragma once

#include "EntityData.h"

#include <set>
#include <string>
#include <iostream>

using std::set;
using std::string;

class Tech {
	set<EntityType> builtTech;
	
public:
	Tech() {}
	bool contains(EntityType t);
	void add(EntityType t);
	void remove(EntityType t);
	bool dependencyFulfilled(const EntityData& e);
	void print();
};

