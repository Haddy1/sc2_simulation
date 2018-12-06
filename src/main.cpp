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

using std::ifstream;
using std::queue;
using std::vector;
using std::pair;
using std::make_pair;


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
		if (!entityExists(s)) {
			validBuildlist = false;
		}
		auto it = entityDataMap.find(nextItem);
		EntityData entityData = it->second;
		if (entityData.race != race) {
			validBuildlist = false;
		}
		
		buildQueue.push(s);
	}
	buildListFile.close();
	
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

	
	JsonLogger logger(race, validBuildlist, "res/output.h");
	logger.printMessage(1, vector<EventEntry>{EventEntry("build-end", "marine"), EventEntry("build-start", "marine")});
	
	ForwardSimulator simulator(race, buildQueue);
	simulator.simulate();
	
	return 0;
}
