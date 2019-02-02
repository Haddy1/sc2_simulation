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
#include <thread>

using std::string;
using std::queue;
using std::pair;
using std::thread;
using std::max;
using std::min;
using std::sort;
using std::set;
using std::vector;




template<typename T>
class Optimizer {
public:
	struct Individual {
		int fitness;
		bool fitnessCalculated;
		queue<T> list;
		
		Individual() : fitness(0), fitnessCalculated(false) {}
		Individual(queue<T> q) : fitness(0), fitnessCalculated(false), list(q) {}
		
		void printList() {
			queue<T> x = list;
			unsigned int size = x.size();
			for (unsigned int i = 0; i < size; ++i) {
				std::clog << x.front() << std::endl;
				x.pop();
			}
		}
		bool operator==(const Individual& other) { return other.list == list; }
		bool operator<(const Individual& j) {
			return fitness < j.fitness;
		}
	};

private:
	vector<Individual> population;
	
	bool rush;
	T target;
	int num;
	Race race;
	
	int startIndex;
	int endIndex;
	
	set<T> searchSpace;
	unordered_map<T, int> searchSpaceLevels;
	
	
	//config:
	long long timeout_ms = 150000;
	int maxGeneration = 50;//50
	int populationSize = 10000;
	int buildListSize = 10;
	int buildListSizeVariation = 20;
	int numSelect = 0.1*populationSize;
	int numMate = 0.4*populationSize;
	int numMutateInsert = 0.25*populationSize;
	int numMutateDelete = 0.25*populationSize;
	int matingPoolSize = 0.1*populationSize;

public:
	
	
	Optimizer(bool rush, T target, int num, Race race) : rush(rush), target(target), num(num), race(race) {
		
	}
	
	
	void init() {
		switch(race) {
		case TERRAN:
			startIndex = T::terran_start;
			endIndex = T::terran_end;
			break;
		case PROTOSS:
			startIndex = T::protoss_start;
			endIndex = T::protoss_end;
			break;
		case ZERG:
			startIndex = T::zerg_start;
			endIndex = T::zerg_end;
			break;
		default:
			break;
		}
		
		//TODO
		//Race specific units which should always be considered to be built
		switch(race) {
		case TERRAN:
			searchSpace.insert(scv);
			searchSpace.insert(supply_depot);
			searchSpace.insert(refinery);
			searchSpace.insert(command_center);
			searchSpace.insert(orbital_command);
			
			break;
		case PROTOSS:
			searchSpace.insert(assimilator);
			searchSpace.insert(pylon);
			searchSpace.insert(probe);
			searchSpace.insert(nexus);
			break;
		case ZERG:
			searchSpace.insert(drone);
			searchSpace.insert(overlord);
			searchSpace.insert(queen);
			searchSpace.insert(extractor);
			searchSpace.insert(hatchery);
			searchSpace.insert(spawning_pool);
			break;
		default:
			break;
		}
		
		addToSetRec(target, 0);
		/*
		std::clog << "search space: " << std::endl;
		for (auto it = searchSpace.begin(); it != searchSpace.end(); ++it) {
			std::clog << entityNameMap.at(*it) << std::endl;
		}
		std::clog << std::endl;
		*/
	}
	
	
	queue<T> optimize() {
		//standard settings
		//config(150000, 50, 10000, 10, 20, 0.1f, 0.8f, 0.05f, 0.05f, 0.1f);
		
		//better for push?
		//config(150000, 100, 10000, 10, 20, 0.1f, 0.4f, 0.25f, 0.25f, 0.1f);
		
		//seems a little better for rush
		//config(150000, 50, 10000, 10, 20, 0.1f, 0.1f, 0.4f, 0.4f, 0.1f);
		
		//test
		//config(150000, 50, 1000, 10, 20, 0.01f, 0.59f, 0.2f, 0.2f, 0.1f);
		
		//all 6 zerg tests pass: (with delete at end only)
		//config(150000, 100, 10000, 10, 20, 0.1f, 0.4f, 0.25f, 0.25f, 0.1f);
		
		//brood lord under 10 min but parse error...
		//config(150000, 100, 40000, 10, 20, 0.1f, 0.4f, 0.25f, 0.25f, 0.1f);
		
		config(150000, 50, 20000, 10, 20, 0.1f, 0.4f, 0.25f, 0.25f, 0.1f);
		
		
		
		Timer timer;
		timer.start();
		srand(0);
		
		
		int numThreads = thread::hardware_concurrency();
		if (numThreads == 0) {
			numThreads = 1;
		}
		//std::clog << "num threads: " << numThreads << std::endl;
		
		int generation = 0;
		
		
		for (int i = 0; i < populationSize; ++i) {
			int r = rand() % buildListSizeVariation;
			queue<T> genome = createGenome(buildListSize + r);
			population.push_back(Individual(genome));
		}
		
		//population[0].printList();
		
		while (true) {
			
			/*
		constexpr int numThreads = 10;
		thread threads[numThreads];
		for (int j = 0; j < numThreads; ++j) {
			threads[j] = thread(&Optimizer::threadFunc, this, numThreads, j);
		}
		for (int j = 0; j < numThreads; ++j) {
			threads[j].join();
		}
		*/
			
			thread *threads = new thread[numThreads];
			for (int j = 0; j < numThreads; ++j) {
				threads[j] = thread(&Optimizer::threadFunc, this, numThreads, j);
			}
			for (int j = 0; j < numThreads; ++j) {
				threads[j].join();
			}
			delete[] threads;
			
			sort(population.begin(), population.end());
			population.erase( unique( population.begin(), population.end() ), population.end() );
			
			std::clog << "Generation " << generation << ", best fitness: " << population[0].fitness << std::endl;
			
			
			//condition for loop end
			if (generation >= maxGeneration || timer.elapsedMilli() > timeout_ms) {
				break;
			}
			
			
			//Selection
			//TODO: select the individuals with -most workers, -most ... as in pdf
			vector<Individual> nextPopulation;
			for (int i = 0; i < numSelect; ++i) {
				nextPopulation.push_back(population[i]);
			}
			
			//Reproduction
			for (int i = 0; i < numMate; ++i) {
				Individual parents[4];
				for (int j = 0; j < 4; ++j) {
					int r = rand() % matingPoolSize;
					parents[j] = population[r];
				}
				pair<int, int> p = pairLargestDistance(parents);
				nextPopulation.push_back(mate(parents[p.first], parents[p.second]));
			}
			
			//Mutation
			//mutate copies of best genomes instead of the offspring by inserting/deleting random genes
			for (int i = 0; i < numMutateDelete; ++i) {
				int r = rand() % numSelect;
				nextPopulation.push_back(mutateDelete(population[r]));
			}
			
			for (int i = 0; i < numMutateInsert; ++i) {
				int r = rand() % numSelect;
				nextPopulation.push_back(mutateInsert(population[r]));
			}
			
			population = nextPopulation;
			
			++generation;
		}
		
		//std::clog << mateFail << ", " << mateNum << std::endl;
		
		//std::clog << "Generations " << generation << ", best fitness: " << population[0].fitness << std::endl;
		
		return population[0].list;
	}
	
	//helpers:
	
	Individual mate(const Individual& a, const Individual& b) {
		// make both parent lists same length
		//mateNum++;
		int n = a.list.size();
		int m = b.list.size();
		queue<T> listA = a.list;
		queue<T> listB = b.list;
		if (n < m) {
			BuildlistValidator validatorA(race, listA);
			validatorA.validate();
			for (int i = n; i < m; ++i) {
				T nextGene = getRandomValidGene(listA, validatorA);
				listA.push(nextGene);
				validatorA.validateNext(nextGene);
			}
		} else if (m < n) {
			BuildlistValidator validatorB(race, listB);
			validatorB.validate();
			for (int i = m; i < n; ++i) {
				T nextGene = getRandomValidGene(listB, validatorB);
				listB.push(nextGene);
				validatorB.validateNext(nextGene);
			}
		}
		
		// generate child list
		queue<T> buildList;
		int size = max(n, m);
		BuildlistValidator validator(race);
		for (int i = 0; i < size; ++i) {
			T nextGene;
			T otherGene;
			if (rand() % 2 == 0) {
				nextGene = listA.front();
				otherGene = listB.front();
			} else {
				nextGene = listB.front();
				otherGene = listA.front();
			}
			listA.pop();
			listB.pop();
			if (validator.checkNext(nextGene)) {
				buildList.push(nextGene);
				validator.validateNext(nextGene);
			} else {
				if (validator.checkNext(otherGene)) {
					buildList.push(otherGene);
					validator.validateNext(otherGene);
				} else {
					//failed, for now return parent A
					//mateFail++;
					return a;
				}
			}
			
		}
		
		return Individual(buildList);
	}
	
	
	
	Individual mutateDelete(const Individual& a) {
		int size = a.list.size();
		if (size == 0) {
			return a;
		}
		int r = rand() % size;
		queue<T> copyList = a.list;
		queue<T> newBuildList;
		for (int i = 0; i < size; ++i) {
			if (i != r) {
				newBuildList.push(copyList.front());
			}
			copyList.pop();
		}
		//validate new buildlist
		BuildlistValidator validator(race, newBuildList);
		if (validator.validate()) {
			return Individual(newBuildList);
		} else {
			return a;
		}
	}
	
	
	Individual mutateInsert(const Individual& a) {
		int size = a.list.size();
		if (size == 0) {
			return a;
		}
		int r = rand() % size;
		queue<T> copyList = a.list;
		queue<T> newBuildList;
		BuildlistValidator validator(race);
		for (int i = 0; i < size; ++i) {
			if (i == r) {
				T randGene = getRandomValidGene(newBuildList, validator);
				newBuildList.push(randGene);
				validator.validateNext(randGene);
			}
			
			if (validator.validateNext(copyList.front())) {
				newBuildList.push(copyList.front());
				copyList.pop();
			} else { //random insert led to rest of buildlist being invalid (eg. due to supply)
				return a;
			}
		}
		return Individual(newBuildList);
	}
	
	int calcDistance(queue<T> a, queue<T> b) {
		int n = min(a.size(), b.size());
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


	
	T getRandomValidGene(const queue<T>& buildList, BuildlistValidator& validator) {
		/*
		vector<string> validChoices;
		for (int i = startIndex; i < endIndex; ++i) {
			const string& entityName = entityNameMap.at(i);
			if (validator.checkNext(entityName)) {
				validChoices.push_back(entityName);
			}
		}
		return validChoices[rand() % validChoices.size()];
		*/
		vector<T> validChoices;
		for (auto it = searchSpace.begin(); it != searchSpace.end(); ++it) {
			const T& entityName = *it;
			if (validator.checkNext(entityName)) {
				validChoices.push_back(entityName);
			}
		}
		return validChoices[rand() % validChoices.size()];
	}
	
	
	
	queue<T> createGenome(int size) {
		//generate at random and make sure the buildlist is valid
		queue<T> buildList;
		
		BuildlistValidator validator(race);
		for (int i = 0; i < size; ++i) {
			T nextGene = getRandomValidGene(buildList, validator);
			buildList.push(nextGene);
			validator.validateNext(nextGene);
		}
		
		return buildList;
	}
	
	
	
	
	void threadFunc(int nthreads, int index) {
		/*
		for (auto it = population.begin(); it != population.end(); ++it) {
			calcFitness(*it);
		}
		*/
		int blocksize = populationSize/nthreads;
		int start = index * blocksize;
		int end = start + blocksize;
		if (index == nthreads - 1) {
			end = populationSize;
		}
		for (int i = start; i < end; ++i) {
			calcFitness(population[i]);
		}
	}
	
	
	
	
	void calcFitness(Individual& ind) {
		//TODO need good fitness function
		if (ind.fitnessCalculated) {
			return;
		}
		
		
		bool timedOut = false;
		int targetUnits = 0;
		int timesteps = 0;
		int numWorkers = 0;
		int numProducers = 0;
		int maxTime;
		if (rush) {
			maxTime = num;
		} else {//push
			maxTime = 1000;
		}
		
		switch(race) {
		case TERRAN:
			{
				TerranSimulator sim(ind.list, false, maxTime);
				sim.init();
				sim.simulate();
				timedOut = sim.timedOut();
				timesteps = sim.getTimeSteps();
				targetUnits = sim.getEntityCount(target);
				break;
			}
		case PROTOSS:
			{
				ProtossSimulator sim(ind.list, false, false, maxTime);
				sim.init();
				sim.simulate();
				targetUnits = sim.numberOfUnits(target);
				timedOut = sim.timedOut();
				timesteps = sim.getTimesteps();
				numWorkers = sim.numberOfWorkers();
				numProducers = sim.numberOfProductionStructures();
				break;
			}
		case ZERG:
			{
				ZergSimulator sim(ind.list, false, maxTime);
				sim.init(); //dont forget init...
				sim.simulate();
				targetUnits = sim.numberOfUnits(target);
				timedOut = sim.timedOut;
				timesteps = sim.timestep;
				break;
			}
		default:
			break;
		}
		
		
		//weird heuristic, but kinda works for push finally. smaller fitness is better
		//for rush, number of built target units determines fitness (only if simulation didnt exceed max time)
		//for push, if number of target units is met, fitness is just the number of timesteps of the simulation
		// if there is not enough or none of the target units then prefer the lists which are closest to the target unit
		if (rush) {
			if (targetUnits == 0 || timedOut) {
				ind.fitness = 2000000000;
				
				//prefer lists close to target in tech tree
				int minLevel = 10;
				queue<T> listCopy = ind.list;
				int size = listCopy.size();
				for (int i = 0; i < size; ++i) {
					T item = listCopy.front();
					listCopy.pop();
					if (searchSpaceLevels.find(item) != searchSpaceLevels.end()) {
						minLevel = min(searchSpaceLevels.at(item), minLevel);
					}
				}
				ind.fitness += minLevel;
				
				
			} else {
				ind.fitness = 2000000000 - targetUnits;
			}
		} else {//push
			if (targetUnits >= num) {
				ind.fitness = timesteps;
			} else {
				ind.fitness = 2000000000 - ((targetUnits + 1) << 8);
				
				//prefer lists close to target in tech tree
				int minLevel = 10;
				queue<T> listCopy = ind.list;
				int size = listCopy.size();
				for (int i = 0; i < size; ++i) {
					T item = listCopy.front();
					listCopy.pop();
					if (searchSpaceLevels.find(item) != searchSpaceLevels.end()) {
						minLevel = min(searchSpaceLevels.at(item), minLevel);
					}
				}
				ind.fitness += minLevel;
				
			}
		}
		
		ind.fitnessCalculated = true;
		
		
	}
	
	
	
	
	void addToSetRec(const T& entityName, int level) {
		if (searchSpace.find(entityName) != searchSpace.end()) { //already in set
			if (searchSpaceLevels.find(entityName) != searchSpaceLevels.end()) {
				searchSpaceLevels.at(entityName) = max(level, searchSpaceLevels.at(entityName));
			}
			return;
		}
		const EntityData& entityData = entityDataMap.at(entityName);
		
		if (entityName != larva) { //avoid adding larva //TODO dont hardcode
			searchSpace.insert(entityName);
			searchSpaceLevels.emplace(entityName, level);
		}
		//searchSpace.insert(entityName);
		
		for (const T& s : entityData.dependencies) {
			addToSetRec(s, level+1);
		}
		for (const T& s : entityData.producedBy) {
			addToSetRec(s, level+1);
		}
	}
	
	void config(
		long long itimeout_ms,
		int imaxGeneration,
		int ipopulationSize,
		int ibuildListSize,
		int ibuildListSizeVariation,
		float fnumSelect,
		float fnumMate,
		float fnumMutateInsert,
		float fnumMutateDelete,
		float fmatingPoolSize)
	{
		timeout_ms = itimeout_ms;
		maxGeneration = imaxGeneration;
		populationSize = ipopulationSize;
		buildListSize = ibuildListSize;
		buildListSizeVariation = ibuildListSizeVariation;
		numSelect = fnumSelect*populationSize;
		numMate = fnumMate*populationSize;
		numMutateInsert = fnumMutateInsert*populationSize;
		numMutateDelete = fnumMutateDelete*populationSize;
		matingPoolSize = fmatingPoolSize*populationSize;
	}
	
	
	
	
	
};


