#include "../include/ForwardSimulator.h"
#include "../include/CSVParser.h"
#include "../include/EntityData.h"
#include "../include/Race.h"
#include "../include/FixedPoint.h" //test
#include "../include/JsonLogger.h"
#include "../include/BuildlistValidator.h"
#include "../include/Timer.h"
#include "../include/Optimizer.h"

#include <iostream>
#include <cstring>
#include <fstream>
#include <queue>
#include <cassert>
#include <string>

using std::ifstream;
using std::queue;
using std::vector;
using std::pair;
using std::make_pair;
using std::string;

Race race;

void usage(char *arg) {
	std::cerr << "usage: " << arg << " <race> <buildListFile>" << std::endl;
}

void optimize(bool rush, string unitname, int number) {
	//std::clog << (rush ? "rush" : "push") << std::endl;
	//std::clog << unitname << ", " << number << std::endl;
	if (!entityExists(unitname)) {
		std::cerr << "invalid unit name" << std::endl;
		return;
	}
	
	EntityType entityType = nameEntityMap.at(unitname);
	EntityData& entityData = entityDataMap.at(entityType);
	race = entityData.race;
	//std::clog << toString(race) << std::endl;
	
	Optimizer<EntityType> opt(rush, entityType, number, race);
	opt.init();
	queue<EntityType> result = opt.optimize();

	
	ForwardSimulator *simulator;
	switch (race) {
		case TERRAN:
			simulator = new TerranSimulator(result, true, 1000);
			break;
		case PROTOSS:
			simulator = new ProtossSimulator(result, true, true, 1000);
			break;
		case ZERG:
			simulator = new ZergSimulator(result, true);
			break;
		default:
			simulator = new ZergSimulator(result, true);
			break;
	}
	
	simulator->init();
	simulator->simulate();
	delete simulator;
}

void forwardSimulate(char *filename) {
	bool invalidBuildlist = false;
	
	// open buildlist file
	ifstream buildListFile(filename);
	
	if (buildListFile.fail()) {
		invalidBuildlist = true;
		std::cerr << "couldn't open buildlist file" << std::endl;
	}
	
	
	//read buildlist from input file
	queue<EntityType> buildQueue;
	while (buildListFile.good()) {
		string s;
		buildListFile >> s;
		if (s.empty()) {
			continue;
		}
		if (!entityExists(s)) {
			invalidBuildlist = true;
			break;
		}
		
		EntityType type = nameEntityMap.at(s);
		EntityData& entityData = entityDataMap.at(type);
		if (entityData.race != race) {
			invalidBuildlist = true;
		}
		
		buildQueue.push(type);
	}
	buildListFile.close();
	
	
	
	BuildlistValidator validator(race, buildQueue);
	invalidBuildlist = invalidBuildlist || (!validator.validate());
	
	
	
	if (invalidBuildlist) {
		std::cout << '{' << std::endl; 
		std::cout << '\t' << '\"' << "buildlistValid" << '\"' << ": " << 0 << ',' << std::endl; 
		std::cout << '\t' << '\"' << "game" << '\"' << ": " << '\"' << toString(race) << '\"' << std::endl;
		std::cout << '}' << std::endl; 
		return;
	}
	
	ForwardSimulator *simulator;
	
	switch (race) {
		case TERRAN:
			simulator = new TerranSimulator(buildQueue, true, 1000);
			break;
		case PROTOSS:
			simulator = new ProtossSimulator(buildQueue, !invalidBuildlist, true, 1000);
			break;
		case ZERG:
			simulator = new ZergSimulator(buildQueue, true);
			break;
		default:
			simulator = new ZergSimulator(buildQueue, true);
			break;
	}
	
	Timer timer;
	
	timer.start();
	simulator->init();
	simulator->simulate();
	
	delete simulator;
	//std::clog << timer.elapsedSec() << " s" << std::endl;
}

int main(int argc, char *argv[]) {
	
	if (argc < 3 || argc > 4) {
		usage(argv[0]);
		return -1;
	}
	
	bool opt = false;
	bool rush = false;
	
	if (strcmp("sc2-hots-terran", argv[1]) == 0 || strcmp("terran", argv[1]) == 0) {
		race = Race::TERRAN;
	} else if (strcmp("sc2-hots-protoss", argv[1]) == 0 || strcmp("protoss", argv[1]) == 0) {
		race = Race::PROTOSS;
	} else if (strcmp("sc2-hots-zerg", argv[1]) == 0 || strcmp("zerg", argv[1]) == 0) {
		race = Race::ZERG;
	} else if (strcmp("rush", argv[1]) == 0) {
		opt = true;
		rush = true;
	} else if (strcmp("push", argv[1]) == 0) {
		opt = true;
	} else {
		usage(argv[0]);
		return -1;
	}
	
	initEntityNameMap();
	
	// read csv
	CSVParser csvp("res/techtrees.csv");
	
	csvp.parse();
	
	
	

	if (opt) {
		optimize(rush, string(argv[2]), atoi(argv[3]));
	} else {
		forwardSimulate(argv[2]);
	}
	
	return 0;
}
