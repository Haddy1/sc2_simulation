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
		bool fitnessCalculated;
		queue<EntityType> list;
	
		Individual() : fitness(0), fitnessCalculated(false) {}
		Individual(queue<EntityType> q) : fitness(0), fitnessCalculated(false), list(q) {}
	
		void printList();
	};

private:
	bool rush;
	EntityType target;
	int num;
	Race race;
	
	int startIndex;
	int endIndex;
	
	set<EntityType> searchSpace;
	unordered_map<EntityType, int> searchSpaceLevels;

public:
	Optimizer(bool rush, EntityType target, int num, Race race);
	void init();
	queue<EntityType> optimize();
	
	//helpers:
	Individual mate(const Individual& a, const Individual& b);
	Individual mutateDelete(const Individual& a);
	Individual mutateInsert(const Individual& a);
	
	template<typename T> int calcDistance(queue<T> a, queue<T> b);
	pair<int, int> pairLargestDistance(Individual inds[4]);
	EntityType getRandomValidGene(const queue<EntityType>& buildList, BuildlistValidator& validator);
	queue<EntityType> createGenome(int size);
	
	void calcFitness(Individual& ind);
	
	void addToSetRec(const EntityType&, int level);
};






