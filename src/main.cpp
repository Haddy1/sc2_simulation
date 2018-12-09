#include "../include/ForwardSimulator.h"
#include "../include/CSVParser.h"
//#include "../include/UnitData.h"
//#include "../include/BuildingData.h"
#include "../include/EntityData.h"
#include "../include/Race.h"
#include "../include/FixedPoint.h" //test
#include "../include/JsonLogger.h"

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

ForwardSimulator *simulator;

void usage(char *arg) {
	std::cerr << "usage: " << arg << " <race> <buildListFile>" << std::endl;
}

int main(int argc, char *argv[]) {
	//cmd line arguments
	if (argc != 3) {
		usage(argv[0]);
		return -1;
	}
	
	Race race;
	
	if (strcmp("sc2-hots-terran", argv[1]) == 0 || strcmp("terran", argv[1]) == 0) {
		std::cout << "1" << std::endl;
		race = Race::TERRAN;
	} else if (strcmp("sc2-hots-protoss", argv[1]) == 0 || strcmp("protoss", argv[1]) == 0) {
		std::cout << "2" << std::endl;
		race = Race::PROTOSS;
	} else if (strcmp("sc2-hots-zerg", argv[1]) == 0 || strcmp("zerg", argv[1]) == 0) {
		std::cout << "3" << std::endl;
		race = Race::ZERG;
	} else {
		usage(argv[0]);
		return -1;
	}

	// read csv
	CSVParser csvp("res/techtrees.csv");
	csvp.parse();
	
	//test print
	for (auto it = entityDataMap.begin(); it != entityDataMap.end(); ++it) {
		std::cout << (*it).second << std::endl;
	}

	std::cout << "end test print" << std::endl;

	// open buildlist file
	ifstream buildListFile(argv[2]);
	//bool invalidBuildlist = buildListFile.fail();
	if (buildListFile.fail()) {
		std::cerr << "couldn't open buildlist file" << std::endl;
	}
	
	bool validBuildlist = true;
	
	queue<string> buildQueue;
	while (buildListFile.good()) {
		string s;
		buildListFile >> s;
		if (s.size() == 0) {
			continue;
		}
		if (!entityExists(s)) {
			validBuildlist = false;
			break;
		}
		
		EntityData entityData = entityDataMap.at(s);
		//auto it = entityDataMap.find(s);
		//EntityData entityData = it->second;
		if (entityData.race != race) {
			validBuildlist = false;
		}
		
		buildQueue.push(s);
	}
	buildListFile.close();
	
	if (!validBuildlist) {
		std::cerr << "invalid buildlist" << std::endl;
	}

	/*
	FixedPoint a(0.0);
	FixedPoint b(0.35);
	FixedPoint c(0.7);
	FixedPoint d = a*b;
	std::cout << "a: "  << a << std::endl;
	std::cout << "b: "  << b << std::endl;
	std::cout << "c: "  << c << std::endl;
	std::cout << "a*b: "  << a*b << std::endl;
	std::cout << "a*c: "  << a*c << std::endl;
	std::cout << "b*c: "  << b*c << std::endl;
	std::cout << "a+b: "  << a+b << std::endl;
	std::cout << "a+c: "  << a+c << std::endl;
	std::cout << "b+c: "  << b+c << std::endl;
	std::cout << "d=a*b: "  << d << std::endl;
	a+= b*FixedPoint(6);
	std::cout << "a+= b*6: " << a << std::endl;
	*/
	
	
	JsonLogger logger(race, validBuildlist, "res/output.txt");
	logger.printMessage(1, vector<EventEntry>{EventEntry("build-end", "marine"), EventEntry("build-start", "marine")});
	
	
	switch (race) {
		case TERRAN:
			simulator = new TerranSimulator(buildQueue);
			break;
		case PROTOSS:
			simulator = new ProtossSimulator(buildQueue);
			break;
		case ZERG:
			simulator = new ZergSimulator(buildQueue);
			break;
		default:
			break;
	}
	
	simulator->simulate();
	
	
	return 0;
}
