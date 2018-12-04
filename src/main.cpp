#include <iostream>
#include <cstring>
#include "../include/ForwardSimulator.h"
#include "../include/CSVParser.h"
#include "../include/UnitData.h"
#include "../include/BuildingData.h"
#include "../include/Race.h"

ForwardSimulator simulator;

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
	
	if (strcmp("sc2-hots-terran", argv[1]) == 0) {
		std::cout << "1" << std::endl;
		race = Race::TERRAN;
	} else if (strcmp("sc2-hots-protoss", argv[1]) == 0) {
		std::cout << "2" << std::endl;
		race = Race::PROTOSS;
	} else if (strcmp("sc2-hots-zerg", argv[1]) == 0) {
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
	for (auto it = unitDataMap.begin(); it != unitDataMap.end(); ++it) {
		std::cout << (*it).second << std::endl;
	}
	std::cout << std::endl;
	for (auto it = buildingDataMap.begin(); it != buildingDataMap.end(); ++it) {
		std::cout << (*it).second << std::endl;
	}

	// open buildlist file
	//
	
	//simulator(race);
	//simulator.simulate();
	return 0;
}
