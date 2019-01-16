#include "../include/Tech.h"

bool Tech::contains(EntityType t) {
	return builtTech.find(t) != builtTech.end();
}

void Tech::add(EntityType t) {
	builtTech.insert(t);
}

void Tech::remove(EntityType t) {
	builtTech.erase(t);
}

bool Tech::dependencyFulfilled(const EntityData& e) {
	if (e.dependencies.empty()) {
		return true;
	}
	for (EntityType dep : e.dependencies) {
		if (contains(dep)) {
			return true;
		}
	}
	return false;
}

void Tech::print() {
	/*
	std::cout << "Tech available: ";
	for (auto it = builtTech.begin(); it != builtTech.end(); ++it) {
		std::cout << (*it) << ", ";
	}
	std::cout << std::endl;
	*/
}