#pragma once

#include <set>
#include <string>

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
