#include "../include/Tech.h"

bool Tech::contains(string s) {
	return builtTech.find(s) != builtTech.end();
}

void Tech::add(string s) {
	builtTech.insert(s);
}

void Tech::remove(string s) {
	builtTech.erase(s);
}

bool Tech::dependencyFulfilled(const EntityData& e) {
	if (e.dependencies.size() == 0) {
		return true;
	}
	for (const string& dep : e.dependencies) {
		if (contains(dep)) {
			return true;
		}
	}
	return false;
}

void Tech::print() {
	std::cout << "Tech available: ";
	for (auto it = builtTech.begin(); it != builtTech.end(); ++it) {
		std::cout << (*it) << ", ";
	}
	std::cout << std::endl;
}