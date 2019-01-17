#include "../include/Optimizer.h"

using std::max;
using std::min;
using std::sort;
using std::set;
using std::vector;

using Individual = Optimizer::Individual;

void Optimizer::calcFitness(Individual& ind) {
	//TODO need good fitness function
	//if (ind.fitnessCalculated) {
	//	return;
	//}
	
	
	bool timedOut = false;
	int targetUnits = 0;
	int timesteps = 0;
	int maxTime;
	if (rush) {
		maxTime = num;
	} else {//push
		maxTime = 1000;
	}
	
	switch(race) {
	case TERRAN:
	{
		//TerranSimulator sim();//TODO
		break;
	}
	case PROTOSS:
	{
		//ProtossSimulator sim(ind.list, false);//TODO
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
	
	
	//ghetto heuristic, but kinda works for push finally. smaller fitness is better
	//for rush, number of built target units determines fitness (only if simulation didnt exceed max time)
	//for push, if number of target units is met, fitness is just the number of timesteps of the simulation
	// if there is not enough or none of the target units then prefer the lists which are closest to the target unit
	if (rush) {
		if (targetUnits == 0 || timedOut) {
			ind.fitness = 2000000000;
		} else {
			ind.fitness = 2000000000/targetUnits;
		}
	} else {//push
		if (targetUnits >= num) {
			ind.fitness = timesteps;
		} else {
			ind.fitness = 2000000000 / (targetUnits + 1);
			
			int minLevel = 1000;
			queue<EntityType> listCopy = ind.list;
			int size = listCopy.size();
			for (int i = 0; i < size; ++i) {
				EntityType item = listCopy.front();
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

void Optimizer::Individual::printList() {
	queue<EntityType> x = list;
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

EntityType Optimizer::getRandomValidGene(const queue<EntityType>& buildList, BuildlistValidator& validator) {
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
	vector<EntityType> validChoices;
	for (auto it = searchSpace.begin(); it != searchSpace.end(); ++it) {
		const EntityType& entityName = *it;
		if (validator.checkNext(entityName)) {
			validChoices.push_back(entityName);
		}
	}
	return validChoices[rand() % validChoices.size()];
}

queue<EntityType> Optimizer::createGenome(int size) {
	//generate at random and make sure the buildlist is valid
	queue<EntityType> buildList;
	
	BuildlistValidator validator(race);
	for (int i = 0; i < size; ++i) {
		EntityType nextGene = getRandomValidGene(buildList, validator);
		buildList.push(nextGene);
		validator.validateNext(nextGene);
	}
	
	return buildList;
}

static int mateNum = 0;
static int mateFail = 0;

Individual Optimizer::mate(const Individual& a, const Individual& b) {
	// make both parent lists same length
	mateNum++;
	int n = a.list.size();
	int m = b.list.size();
	queue<EntityType> listA = a.list;
	queue<EntityType> listB = b.list;
	if (n < m) {
		BuildlistValidator validatorA(race, listA);
		validatorA.validate();
		for (int i = n; i < m; ++i) {
			EntityType nextGene = getRandomValidGene(listA, validatorA);
			listA.push(nextGene);
			validatorA.validateNext(nextGene);
		}
	} else if (m < n) {
		BuildlistValidator validatorB(race, listB);
		validatorB.validate();
		for (int i = m; i < n; ++i) {
			EntityType nextGene = getRandomValidGene(listB, validatorB);
			listB.push(nextGene);
			validatorB.validateNext(nextGene);
		}
	}
	
	// generate child list
	queue<EntityType> buildList;
	int size = max(n, m);
	BuildlistValidator validator(race);
	for (int i = 0; i < size; ++i) {
		EntityType nextGene;
		EntityType otherGene;
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
				mateFail++;
				return a;
			}
		}
		
	}
	
	return Individual(buildList);
}

Individual Optimizer::mutateDelete(const Individual& a) {
	int size = a.list.size();
	if (size == 0) {
		return a;
	}
	int r = rand() % size;
	queue<EntityType> copyList = a.list;
	queue<EntityType> newBuildList;
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

//insert random at end of list
Individual Optimizer::mutateInsert(const Individual& a) {
	//int size = a.list.size();
	queue<EntityType> newBuildList = a.list;
	BuildlistValidator validator(race, newBuildList);
	validator.validate();
	
	EntityType randGene = getRandomValidGene(newBuildList, validator);
	newBuildList.push(randGene);
	validator.validateNext(randGene);
	return Individual(newBuildList);
}

/*
Individual Optimizer::mutateInsert(const Individual& a) {
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
		
		if (validator.validateNext(copyList.front())) {
			newBuildList.push(copyList.front());
			copyList.pop();
		} else { //random insert led to rest of buildlist being invalid (eg. due to supply)
			return a;
		}
	}
	return Individual(newBuildList);
}
*/

bool operator<(const Individual& i, const Individual& j) {
	return i.fitness < j.fitness;
}

Optimizer::Optimizer(bool rush, EntityType target, int num, Race race) : rush(rush), target(target), num(num), race(race) {
	
}

void Optimizer::addToSetRec(const EntityType& entityName, int level) {
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
	
	for (const EntityType& s : entityData.dependencies) {
		addToSetRec(s, level+1);
	}
	for (const EntityType& s : entityData.producedBy) {
		addToSetRec(s, level+1);
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
	
	//TODO
	//Race specific units which should always be considered to be built
	switch(race) {
	case TERRAN:
		
		break;
	case PROTOSS:
		
		break;
	case ZERG:
		searchSpace.insert(drone);
		searchSpace.insert(overlord);
		searchSpace.insert(queen);
		searchSpace.insert(extractor);
		searchSpace.insert(hatchery);
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

void Optimizer::threadFunc(int nthreads, int index) {
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

queue<EntityType> Optimizer::optimize() {
	//standard settings
	//config(150000, 50, 10000, 10, 20, 0.1f, 0.8f, 0.05f, 0.05f, 0.1f);
	
	//better for push?
	config(150000, 50, 10000, 10, 20, 0.1f, 0.4f, 0.25f, 0.25f, 0.1f);
	
	//seems a little better for rush
	//config(150000, 50, 10000, 10, 20, 0.1f, 0.1f, 0.4f, 0.4f, 0.1f);
	
	//test
	//config(150000, 50, 1000, 10, 20, 0.01f, 0.59f, 0.2f, 0.2f, 0.1f);
	
	
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
		queue<EntityType> genome = createGenome(buildListSize + r);
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
	
		//std::clog << "Generation " << generation << ", best fitness: " << population[0].fitness << std::endl;
		
		
		//condition for loop end
		if (generation > maxGeneration || timer.elapsedMilli() > timeout_ms) {
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
	
	return population[0].list;
}

void Optimizer::config(
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






