#pragma once

#include "EntityData.h"

#include <string>

using std::string;

class Entity {
protected:
	int id;
	EntityData *entityData;
public:
	Entity(int& ID_Counter, EntityType type);
	int getID() const;
	const EntityData *getEntityData() const;
	const string& getName() const;
	const EntityType getType() const;
	const int getBuildTime() const;
};
