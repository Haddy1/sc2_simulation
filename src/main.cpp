#include "../include/ForwardSimulator.h"
#include "../include/CSVParser.h"
#include "../include/EntityData.h"
#include "../include/Race.h"
#include "../include/FixedPoint.h" //test
#include "../include/JsonLogger.h"
#include "../include/BuildlistValidator.h"
#include "../include/Timer.h"

#include <iostream>
#include <cstring>
#include <fstream>
#include <queue>
#include <cassert>

using std::ifstream;
using std::queue;
using std::vector;
using std::pair;
using std::make_pair;


void usage(char *arg) {
	std::cerr << "usage: " << arg << " <race> <buildListFile>" << std::endl;
}

int main(int argc, char *argv[]) {
	
	if (argc != 3) { //TODO 4 args with optimizer
		usage(argv[0]);
		return -1;
	}
	
	Race race;
	
	if (strcmp("sc2-hots-terran", argv[1]) == 0 || strcmp("terran", argv[1]) == 0) {
		race = Race::TERRAN;
	} else if (strcmp("sc2-hots-protoss", argv[1]) == 0 || strcmp("protoss", argv[1]) == 0) {
		race = Race::PROTOSS;
	} else if (strcmp("sc2-hots-zerg", argv[1]) == 0 || strcmp("zerg", argv[1]) == 0) {
		race = Race::ZERG;
	} else if (strcmp("rush", argv[1]) == 0) {
		//TODO
		return 0;
	} else if (strcmp("push", argv[1]) == 0) {
		//TODO
		return 0;
	} else {
		usage(argv[0]);
		return -1;
	}

	// read csv
    CSVParser csvp("res/techtrees.csv");
	csvp.parse();
	
	initEntityNameMap();
	
	/*
	for (auto it = entityDataMap.begin(); it != entityDataMap.end(); ++it) {
		std::cout << (*it).second << std::endl;
	} */
	
	bool invalidBuildlist = false;
	
	// open buildlist file
	ifstream buildListFile(argv[2]);
	
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
		
		//std::cerr << "invalid buildlist" << std::endl;
		return 0;
	}

	/*
	FixedPoint a(0.01);
	FixedPoint b(0.35);
	FixedPoint c(0.7);
	std::cout << "a: "  << a << std::endl;
	std::cout << "b: "  << b << std::endl;
	std::cout << "c: "  << c << std::endl;
	std::cout << "a*b: "  << a*b << std::endl;
	std::cout << "a*c: "  << a*c << std::endl;
	std::cout << "b*c: "  << b*c << std::endl;
	std::cout << "a+b: "  << a+b << std::endl;
	std::cout << "a+c: "  << a+c << std::endl;
	std::cout << "b+c: "  << b+c << std::endl;
	FixedPoint d = a*b;
	std::cout << "d=a*b: "  << d << std::endl;
	a+= b*FixedPoint(6);
	std::cout << "a+= b*6: " << a << std::endl;
	*/
	/*
	FixedPoint a(0.0);
	FixedPoint b(1.0);
	FixedPoint c(2.0);
	FixedPoint d(1.1);
	FixedPoint e(2.9);
	std::cout << "FP test: " << (a*2) << std::endl;
	std::cout << "FP test: " << (b*2) << std::endl;
	std::cout << "FP test: " << (c*2) << std::endl;
	std::cout << "FP test: " << (d*2) << std::endl;
	std::cout << "FP test: " << (e*2) << std::endl;
	*/
	
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
	
	return 0;
}
