#include "../include/Optimizer.h"

using std::max;
using std::min;
using std::sort;
using std::set;
using std::vector;

using Individual = Optimizer::Individual;

void Optimizer::Individual::calcFitness(bool rush, string target, int num, Race race) {
	//TODO need good fitness function
	switch(race) {
	case TERRAN:
	{
		TerranSimulator sim();//TODO
		break;
	}
	case PROTOSS:
	{
		ProtossSimulator sim(list, false);//TODO
		break;
	}
	case ZERG:
	{
		int maxTime;
		if (rush) {
			maxTime = num;
		} else {//push
			maxTime = 1000;
		}
		
		ZergSimulator sim(list, false, maxTime);
		sim.init(); //dont forget init...
		sim.simulate();
		int targetUnits = sim.numberOfUnits(target);
		
		if (rush) {
			//if (targetUnits == 0) {
			if (targetUnits == 0 || sim.timedOut) {
				fitness = 2000000;
			} else {
				fitness = 100000/targetUnits;
			}
		} else {//push
			if (targetUnits >= num) {
				fitness = sim.timestep;
			} else {
				fitness = 2000000;
			}
		}
		break;
	}
	default:
		break;
	}
	
	
}

void Optimizer::Individual::printList() {
	queue<string> x = list;
	unsigned int size = x.size();
	for (unsigned int i = 0; i < size; ++i) {
		std::clog << x.front() << std::endl;
		x.pop();
	}
}

//TODO use vectors
template<typename T>
int Optimizer::calcDistance(queue<T> a, queue<T> b) {
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

pair<int, int> Optimizer::pairLargestDistance(Individual inds[4]) {
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

string Optimizer::getRandomValidGene(const queue<string>& buildList, BuildlistValidator& validator, Race race) {
	//BuildlistValidator v(race, buildList);
	//validator.validate();
	vector<string> validChoices;
	for (int i = startIndex; i < endIndex; ++i) {
		const string& entityName = entityNameMap.at(i);
		if (validator.checkNext(entityName)) {
			validChoices.push_back(entityName);
		}
	}
	return validChoices[rand() % validChoices.size()];
}

queue<string> Optimizer::createGenome(Race race, int size) {
	//generate at random and make sure the buildlist is valid
	queue<string> buildList;
	
	//int range = end - start;
	//int randIndex = (rand() % range) + start;
	BuildlistValidator validator(race);
	for (int i = 0; i < size; ++i) {
		string nextGene = getRandomValidGene(buildList, validator, race);
		buildList.push(nextGene);
		validator.validateNext(nextGene);
	}
	
	return buildList;
}

Individual Optimizer::mate(const Individual& a, const Individual& b, Race race) {
	
	
	// make both parent lists same length
	int n = a.list.size();
	int m = b.list.size();
	queue<string> listA = a.list;
	queue<string> listB = b.list;
	if (n < m) {
		BuildlistValidator validatorA(race, listA);
		validatorA.validate();
		for (int i = n; i < m; ++i) {
			string nextGene = getRandomValidGene(listA, validatorA, race);
			listA.push(nextGene);
			validatorA.validateNext(nextGene);
		}
	} else if (m < n) {
		BuildlistValidator validatorB(race, listB);
		validatorB.validate();
		for (int i = m; i < n; ++i) {
			string nextGene = getRandomValidGene(listB, validatorB, race);
			listB.push(nextGene);
			validatorB.validateNext(nextGene);
		}
	}
	
	// generate child list
	queue<string> buildList;
	int size = max(n, m);
	BuildlistValidator validator(race);
	for (int i = 0; i < size; ++i) {
		string nextGene;
		string otherGene;
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
				return a;
			}
		}
		
	}
	
	return Individual(buildList);
}

Individual Optimizer::mutateDelete(const Individual& a, Race race) {
	int size = a.list.size();
	int r = rand() % size;
	queue<string> copyList = a.list;
	queue<string> newBuildList;
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

Individual Optimizer::mutateInsert(const Individual& a, Race race) {
	int size = a.list.size();
	int r = rand() % size;
	queue<string> copyList = a.list;
	queue<string> newBuildList;
	BuildlistValidator validator(race);
	for (int i = 0; i < size; ++i) {
		if (i == r) {
			string randGene = getRandomValidGene(newBuildList, validator, race);
			newBuildList.push(randGene);
			validator.validateNext(randGene);
		}
		newBuildList.push(copyList.front());
		validator.validateNext(copyList.front());
		copyList.pop();
	}
	return newBuildList;
}

bool operator<(const Individual& i, const Individual& j) {
	return i.fitness < j.fitness;
}

Optimizer::Optimizer(bool rush, string target, int num, Race race) : rush(rush), target(target), num(num), race(race) {
	
}

void Optimizer::addToSetRec(const string& entityName) {
	if (searchSpace.find(entityName) != searchSpace.end()) { //already in set
		return;
	}
	const EntityData& entityData = entityDataMap.at(entityName);
	//if (entityExists(entityName) { //avoid adding larva
	//	searchSpace.insert(entityName);
	//}
	searchSpace.insert(entityName);
	for (const string& s : entityData.dependencies) {
		addToSetRec(s);
	}
	for (const string& s : entityData.producedBy) {
		addToSetRec(s);
	}
}

void Optimizer::init() {
	switch(race) {
	case TERRAN:
		startIndex = EntityType::terran_start;
		endIndex = EntityType::terran_end;
		break;
	case PROTOSS:
		startIndex = EntityType::protoss_start;
		endIndex = EntityType::protoss_end;
		break;
	case ZERG:
		startIndex = EntityType::zerg_start;
		endIndex = EntityType::zerg_end;
		break;
	default:
		break;
	}
	
	
	addToSetRec(target);
}

queue<string> Optimizer::optimize() {
	Timer timer;
	timer.start();
	
	srand(0);
	//rand() between 0 and RAND_MAX
	bool found = false;
	int generation = 0;
	int maxGeneration = 50;
	int populationSize = 10000;
	int numSelect = populationSize/10;
	int numMate = 8*populationSize/10;
	int matingPoolSize = populationSize/10;
	int buildListSize = 10;
	long timeout = 179000;
	
	vector<Individual> population;
	
	for (int i = 0; i < populationSize; ++i) {
		int listSizeVariation = rand() % 20;
		queue<string> genome = createGenome(race, buildListSize + listSizeVariation);
		population.push_back(Individual(genome));
	}
	
	while (!found) {
		for (auto it = population.begin(); it != population.end(); ++it) {
			it->calcFitness(rush, target, num, race);
		}
		
		sort(population.begin(), population.end());
		
	
		std::clog << "best fitness: " << population[0].fitness << std::endl;
		std::clog << "worst fitness: " << population[9999].fitness << std::endl;
		//std::clog << population[0].list.size() << std::endl;
		//std::clog << population[9999].list.size() << std::endl;
		//condition for loop end
		
		if (generation > maxGeneration || timer.elapsedMilli() > timeout) {
			//found = true;
			break;
		}
		
		
		//Selection
		//TODO: select the individuals with -most workers, -most ... as in pdf
		vector<Individual> nextPopulation;
		for (int i = 0; i < numSelect; ++i) {
			nextPopulation.push_back(population[i]);
		}
		
		//Reproduction
		for (int i = numSelect; i < numMate + numSelect; ++i) {
			Individual parents[4];
			for (int j = 0; j < 4; ++j) {
				int r = rand() % matingPoolSize;
				parents[j] = population[r];
			}
			pair<int, int> p = pairLargestDistance(parents);
			nextPopulation.push_back(mate(parents[p.first], parents[p.second], race));
		}
		
		//Mutation
		//mutate copies of best genomes instead of the offspring by inserting/deleting random genes
		for (int i = numMate + numSelect; i < 95*populationSize/100; ++i) {
			int r = rand() % numSelect;
			nextPopulation.push_back(mutateDelete(population[r], race));
		}
		for (int i = 95*populationSize/100; i < populationSize; ++i) {
			int r = rand() % numSelect;
			nextPopulation.push_back(mutateInsert(population[r], race));
		}
		
		
		population = nextPopulation;
		
		std::clog << "Generation " << generation << ", Population " << population.size() << std::endl;
		
		++generation;
	}
	
	return population[0].list;
}






