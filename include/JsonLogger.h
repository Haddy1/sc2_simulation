#include "../include/EntityData.h"
#include "../include/Race.h"
#include "../include/ResourceManager.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>
#include <utility>

#pragma once

using std::cout;
using std::endl;
using std::vector;
using std::make_pair;
using std::pair;

class EventEntry {
public:
	EventEntry(string s1, string s2) : val(make_pair(s1, s2)) {}
	string first() {
		return val.first;
	}
	string second() {
		return val.second;
	}
private:
	pair<string, string> val;
};

class JsonLogger {
public:
	JsonLogger(Race, ResourceManager&, bool);
	JsonLogger(Race, ResourceManager&, bool, std::string);
	~JsonLogger();
	
	void printMessage(int, vector<EventEntry>);
	
private:
	Race race;
	ResourceManager& rm;
	bool validBuildlist;
	
	std::string path;
	char ws = '\t'; // changeable whitespace character
	
	std::ofstream out;
	std::streambuf* coutbuf;
	
	void printBeginning();
	void redirect();
	void undo_redirect();
	
	bool validType(const char*);
	bool validName(string);
};
