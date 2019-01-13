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
	std::clog << (rush ? "rush" : "push") << std::endl;
	std::clog << unitname << ", " << number << std::endl;
	if (!entityExists(unitname)) {
		std::cerr << "invalid unit name" << std::endl;
		return;
	}
	EntityData& entityData = entityDataMap.at(unitname);
	race = entityData.race;
	std::clog << toString(race) << std::endl;
	
	Optimizer opt(rush, unitname, number, race);
	opt.init();
	queue<string> result = opt.optimize();
	
	ForwardSimulator *simulator;
	switch (race) {
		case TERRAN:
			simulator = new TerranSimulator(result);
			break;
		case PROTOSS:
			simulator = new ProtossSimulator(result, true);
			break;
		case ZERG:
			simulator = new ZergSimulator(result, true);
			break;
		default:
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
	
	
	
	queue<string> buildQueue;
	while (buildListFile.good()) {
		string s;
		buildListFile >> s;
		if (s.size() == 0) {
			continue;
		}
		if (!entityExists(s)) {
			invalidBuildlist = true;
			break;
		}
		
		EntityData& entityData = entityDataMap.at(s); //always use reference to EntityData
		if (entityData.race != race) {
			invalidBuildlist = true;
		}
		
		buildQueue.push(s);
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
			simulator = new TerranSimulator(buildQueue);
			break;
		case PROTOSS:
			simulator = new ProtossSimulator(buildQueue, !invalidBuildlist);
			break;
		case ZERG:
			simulator = new ZergSimulator(buildQueue, true);
			break;
		default:
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

	// read csv
	CSVParser csvp("res/techtrees.csv");
	csvp.parse();
	
	initEntityNameMap();
	
	if (opt) {
		optimize(rush, string(argv[2]), atoi(argv[3]));
	} else {
		forwardSimulate(argv[2]);
	}
	
	return 0;
}
