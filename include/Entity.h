#pragma once

#include "EntityData.h"

#include <string>

using std::string;

class Entity {
protected:
	int id;
	EntityData *entityData;
public:
	Entity(string name);
	int getID() const;
	const EntityData *getEntityData() const;
	const string& getName() const;
};
