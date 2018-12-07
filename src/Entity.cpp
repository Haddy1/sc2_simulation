#include "../include/Entity.h"

static int ID_Counter = 0;

Entity::Entity() {
	id = ID_Counter++;
}

int Entity::getID() const {
	return id;
}
