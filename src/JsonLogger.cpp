#include "../include/JsonLogger.h"

// log into cout
JsonLogger::JsonLogger(Race r, ResourceManager manager, bool valid) : race(r), rm(manager), validBuildlist(valid)  {
	printBeginning();
}

// log into file by redirecting from cout to file(path)
JsonLogger::JsonLogger(Race r, ResourceManager manager, bool valid, std::string str) : race(r), rm(manager), validBuildlist(valid), path(str), out(str.c_str()), coutbuf(cout.rdbuf()) {	
	printBeginning();
}

JsonLogger::~JsonLogger() {
	redirect();
	if(validBuildlist) {
		cout << endl << ws << "]\n"; // ends the "messages" block
	}
	if(race != NONE) cout << "}\n";
	undo_redirect();
	
	if(!path.empty()) {
		out.flush();
		out.close();
	}
}

void JsonLogger::redirect() {
	if(!path.empty())
		cout.rdbuf(out.rdbuf());
}

void JsonLogger::undo_redirect() {
	if(!path.empty())
		cout.rdbuf(coutbuf);
}

void JsonLogger::printBeginning() {
	redirect();
	cout << "{\n"; 
	cout << ws << "\"buildlistValid\": " << (int) validBuildlist << ",\n"; 
	cout << ws << "\"game\": \"" << toString(race) << "\",\n";
	
	if(validBuildlist) {
		cout << ws << "\"messages\": [\n";
	}
	undo_redirect();
}

// TODO special ability events and initial units
void JsonLogger::printMessage(int time, vector<EventEntry> events) {
	assert(time > 0);
	// messages not added to log for invalid build lists
	if(!validBuildlist) {
		return;
	}
	redirect();
	// add message entry to output
	if(time > 1) {
		cout << ",\n"; // separation with previous message entry
	}
	cout << string(2, ws) << "{\n";
	// time
	cout << string(3, ws) << "\"time\": " << time << ",\n";
	// status
	cout << string(3, ws) << "\"status\": {\n";
	
	cout << string(4, ws) << "\"workers\": {\n";
	cout << string(5, ws) << "\"minerals\": " << rm.getMineralWorkers() << ",\n";
	cout << string(5, ws) << "\"vespene\": " << rm.getVespeneWorkers() << "\n";
	cout << string(4, ws) << "},\n";
	
	cout << string(4, ws) << "\"resources\": {\n";
	cout << string(5, ws) << "\"minerals\": " << rm.getMinerals() << ",\n";
	cout << string(5, ws) << "\"vespene\": " << rm.getVespene() << ",\n";
	cout << string(5, ws) << "\"supply-used\": " << rm.getSupply() << ",\n";
	cout << string(5, ws) << "\"supply\": " << rm.getSupplyMax() << "\n";
	cout << string(4, ws) << "}\n";
	
	cout << string(3, ws) << "},\n";
	// events
	cout << string(3, ws) << "\"events\": [\n";
	for(auto& event : events) {
		//if(!validType(event.first().c_str()) || !validName(event.second())) { continue;}
		cout << string(4, ws) << "{\n";
		cout << string(5, ws) << "\"type\": " << event.first() << ",\n";
		cout << string(5, ws) << "\"name\": " << event.second() << "\n";
		cout << string(4, ws) << "}\n";
		
		if(&event != &events.back()) {
			cout << string(4, ws) << ",\n";
		}
	}
	cout << string(2, ws) << "}";
	undo_redirect();
}

bool JsonLogger::validType(const char* s) {
	return strcmp(s, "build-start") == 0 || strcmp(s, "build-end") == 0 || strcmp(s, "special") == 0;
}

bool JsonLogger::validName(string s) {
	return entityDataMap.find(s) != entityDataMap.end();
}
