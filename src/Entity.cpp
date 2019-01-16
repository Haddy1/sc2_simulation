#include "../include/Entity.h"

//static int ID_Counter = 0;

Entity::Entity(int& ID_Counter, EntityType type) : entityData(&entityDataMap.at(type)) {
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

const EntityType Entity::getType() const {
	return entityData->type;
}

const int Entity::getBuildTime() const {
	return entityData->buildTime;
}
