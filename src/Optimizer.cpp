#include "../include/Optimizer.h"
#include "../include/Timer.h"
#include "../include/EntityData.h"

#include <algorithm>
#include <cstdlib>

using std::max;
using std::sort;

Optimizer::Optimizer(Race race) : race(race) {
	
}

//TODO use vectors
template<typename T>
int calcDistance(queue<T> a, queue<T> b) {
	int n = a.size();
	int d = 0;
	for (int i = 0; i < n; ++i) {
		T ai = a.front();
		a.pop();
		T bi = b.front();
		b.pop();
		if (ai != bi) {
			d += (n - (i + 1));
		}
	}
	return d;
}

pair<int, int> pairLargestDistance(Individual inds[4]) {
	int d[6];
	d[0] = calcDistance(inds[0].list, inds[1].list);
	d[1] = calcDistance(inds[0].list, inds[2].list);
	d[2] = calcDistance(inds[0].list, inds[3].list);
	d[3] = calcDistance(inds[1].list, inds[2].list);
	d[4] = calcDistance(inds[1].list, inds[3].list);
	d[5] = calcDistance(inds[2].list, inds[3].list);
	int m = 0;
	int mindex = 0;
	for (int i = 0; i < 6; ++i) {
		if (d[i] > m) {
			m = d[i];
			mindex = i;
		}
	}
	switch(mindex) {
		case 0: return pair<int, int>(0,1);
		case 1: return pair<int, int>(0,2);
		case 2: return pair<int, int>(0,3);
		case 3: return pair<int, int>(1,2);
		case 4: return pair<int, int>(1,3);
		default: return pair<int, int>(2,3);
	}
}

bool operator<(const Individual& i, const Individual& j) {
	return i.fitness < j.fitness;
}

queue<string> createGenome(Race race) {
	//TODO
	//generate at random and make sure the buildlist is valid
	queue<string> buildList;
	
	int start = 0;
	int end = 0;
	switch(race) {
		case TERRAN:
		start = EntityType::terran_start;
		end = EntityType::terran_end;
		break;
		case PROTOSS:
		start = EntityType::protoss_start;
		end = EntityType::protoss_end;
		break;
		case ZERG:
		start = EntityType::zerg_start;
		end = EntityType::zerg_end;
		break;
		default:
		break;
	}
	int range = end - start;
	int randIndex = (rand() % range) + start;
	
	const string& entityName = entityNameMap.at(randIndex);
	
	//check if valid choice
	//...
	
	
	buildList.push(entityName);
	
	//BuildlistValidator validator(race, buildList);
	//validator.validate();
	return buildList;
}

void Individual::calcFitness() {
	//TODO
	//this->fitness = x;
}

Individual mate(Individual& a, Individual& b) {
	//TODO
	return Individual(a.list);
}

void Optimizer::optimize() {
	Timer timer;
	timer.start();
	timer.elapsedMilli(); // end algorithm when just under 180 sec
	
	
	
	srand(0);
	//rand() between 0 and RAND_MAX
	bool found = false;
	int generation = 0;
	int populationSize = 1000;
	int numSelect = populationSize/10;
	int matingPoolSize = populationSize/10;
	
	vector<Individual> population;
	
	for (int i = 0; i < populationSize; ++i) {
		queue<string> genome = createGenome(race);
		population.push_back(Individual(genome));
	}
	
	while (!found) {
		for (auto it = population.begin(); it != population.end(); ++it) {
			it->calcFitness();
		}
		
		sort(population.begin(), population.end());
		
		//condition for loop end
		/*
		if () {
			found = true;
			break;
		}
		*/
		
		//Selection
		//TODO: select the individuals with -most workers, -most ... as in pdf
		vector<Individual> nextPopulation;
		for (int i = 0; i < numSelect; ++i) {
			nextPopulation.push_back(population[i]);
		}
		
		//Reproduction
		for (int i = numSelect; i < populationSize; ++i) {
			Individual parents[4];
			for (int j = 0; j < 4; ++j) {
				int r = rand() % matingPoolSize;
				parents[j] = population[r];
			}
			pair<int, int> p = pairLargestDistance(parents);
			nextPopulation.push_back(mate(parents[p.first], parents[p.second]));
		}
		
		//Mutation
		
		
		
		population = nextPopulation;
		
		++generation;
	}
}






