#pragma once

#include "EntityData.h"

#include <set>
#include <string>
#include <iostream>

using std::set;
using std::string;

class Tech {
	set<string> builtTech;
	
public:
	Tech() {}
	bool contains(string s);
	void add(string s);
	void remove(string s);
	bool dependencyFulfilled(const EntityData& e);
	void print();
};

