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
	void calcFitness(Race race);
};

class Optimizer {
	Race race;
public:
	Optimizer(Race race);
	queue<string> optimize();
	
};



