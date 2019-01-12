#include "../include/Entity.h"

static int ID_Counter = 0;

Entity::Entity(string name) : entityData(&entityDataMap.at(name)) {
	id = ID_Counter++;
}

int Entity::getID() const {
	return id;
}

const EntityData *Entity::getEntityData() const {
	return entityData;
}

const string& Entity::getName() const {
	return entityData->name;
}
