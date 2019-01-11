#pragma once

#include "Race.h"

#include <string>
#include <queue>
#include <utility>

using std::string;
using std::queue;
using std::pair;

struct Individual {
	int fitness;
	queue<string> list;
	
	Individual() {}
	Individual(queue<string> q) : list(q) {}
	void calcFitness(bool rush, string target, int num, Race race);
	void printList();
};

class Optimizer {
	bool rush;
	string target;
	int num;
	Race race;
public:
	Optimizer(bool rush, string target, int num, Race race);
	queue<string> optimize();
	
};



