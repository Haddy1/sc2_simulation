#pragma once

#include "EntityData.h"

#include <set>
#include <string>
#include <iostream>

using std::set;
using std::string;

extern set<string> builtTech;

inline bool techContains(string s) {
	return builtTech.find(s) != builtTech.end();
}

inline void techAdd(string s) {
	builtTech.insert(s);
}

inline void techRemove(string s) {
	builtTech.erase(s);
}

inline bool dependencyFulfilled(EntityData& e) {
	if (e.dependencies.size() == 0) {
		return true;
	}
	for (string& dep : e.dependencies) {
		if (techContains(dep)) {
			return true;
		}
	}
	return false;
}

inline void techPrint() {
	std::cout << "Tech available: ";
	for (auto it = builtTech.begin(); it != builtTech.end(); ++it) {
		std::cout << (*it) << ", ";
	}
	std::cout << std::endl;
}
