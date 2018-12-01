#include <iostream>
#include <cstring>
#include "../include/simulation.h"
#include "../include/test.h"

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

	// open buildlist file
	//
	
	test();
	test1();
	return 0;
}
