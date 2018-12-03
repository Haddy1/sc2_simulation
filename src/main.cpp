#include <iostream>
#include <cstring>
#include "../include/ForwardSimulator.h"
#include "../include/CSVParser.h"
#include "../include/UnitData.h"
#include "../include/BuildingData.h"

void usage(char *arg) {
	std::cerr << "usage: " << arg << " <race> <buildListFile>" << std::endl;
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		usage(argv[0]);
		return -1;
	}
	
	if (strcmp("sc2-hots-terran", argv[1]) == 0) {
		std::cout << "1" << std::endl;
	} else if (strcmp("sc2-hots-protoss", argv[1]) == 0) {
		std::cout << "2" << std::endl;
	} else if (strcmp("sc2-hots-zerg", argv[1]) == 0) {
		std::cout << "3" << std::endl;
	} else {
		usage(argv[0]);
		return -1;
	}

	// read csv
	CSVParser csvp("res/techtrees.csv");
	csvp.read();
	
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
	
	simulateTimestep();
	return 0;
}
