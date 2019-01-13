#pragma once

#include "Race.h"
#include "Timer.h"
#include "EntityData.h"
#include "BuildlistValidator.h"
#include "ForwardSimulator.h"

#include <algorithm>
#include <cstdlib>
#include <set>
#include <vector>
#include <iostream>
#include <string>
#include <queue>
#include <utility>

using std::string;
using std::queue;
using std::pair;



class Optimizer {
public:
	struct Individual {
		int fitness;
		queue<string> list;
	
		Individual() {}
		Individual(queue<string> q) : list(q) {}
	
		void calcFitness(bool rush, string target, int num, Race race);
		void printList();
	};

private:
	bool rush;
	string target;
	int num;
	Race race;
	
	int startIndex;
	int endIndex;
	
	set<string> searchSpace;

public:
	Optimizer(bool rush, string target, int num, Race race);
	void init();
	queue<string> optimize();
	
	//helpers:
	Individual mate(const Individual& a, const Individual& b, Race race);
	Individual mutateDelete(const Individual& a, Race race);
	Individual mutateInsert(const Individual& a, Race race);
	
	template<typename T> int calcDistance(queue<T> a, queue<T> b);
	pair<int, int> pairLargestDistance(Individual inds[4]);
	string getRandomValidGene(const queue<string>& buildList, BuildlistValidator& validator, Race race);
	queue<string> createGenome(Race race, int size);
	
	void addToSetRec(const string&);
};






