#include "JsonLogger.h"

// log into cout
JsonLogger::JsonLogger(Race r, ResourceManager& manager, bool valid) : race(r), rm(manager), validBuildlist(valid), firstMsg(true)  {
	if(valid) {
		printBeginning();
	}
}

// log into file by redirecting from cout to file(path)
JsonLogger::JsonLogger(Race r, ResourceManager& manager, bool valid, std::string str) : race(r), rm(manager), validBuildlist(valid), firstMsg(true), path(str), out(str.c_str()), coutbuf(cout.rdbuf()) {	
	if(valid) {
		printBeginning();
	}
}

JsonLogger::~JsonLogger() {
	redirect();
	if(validBuildlist) {
		cout << endl << ws << "]" << endl; // ends the "messages" block
		cout << "}" << endl;
	}
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
	cout << "{" << endl; 
	cout << ws << "\"buildlistValid\": " << (int) validBuildlist << "," << endl; 
	cout << ws << "\"game\": \"" << toString(race) << "\"" << (validBuildlist ? "," : "") << endl;
	undo_redirect();
}

void JsonLogger::printSetup(vector<pair<string, vector<int>>>& entities) {
	// print initial units
	redirect();
	if(validBuildlist) {
		cout << ws << "\"initialUnits\" : {" << endl;
		for(auto& unit : entities) {
			cout << string(2, ws) << "\"" << unit.first << "\" : [" << endl;
			for(auto& id : unit.second) {
				cout << string(3, ws) << "\"" << id << "\"";
				cout << (id != unit.second.back() ? ",\n" : string("\n" + string(2, ws) + "]"));
			}
			cout << (unit != entities.back() ? ", " : "") << endl;
		}
		cout << ws << "}," << endl;
	}
	undo_redirect();
}

void JsonLogger::printMessage(int time, vector<shared_ptr<EventEntry>>& events) {
	// messages not added to log for invalid build lists
	if(!validBuildlist || events.empty()) {
		return;
	}
	redirect();
	// add message entry to output
	if(!firstMsg) {
		cout << "," << endl; // separation with previous message entry
	}
	firstMsg = false;
	cout << string(2, ws) << "{" << endl;
	// time
	cout << string(3, ws) << "\"time\": " << time << "," << endl;
	// status
	cout << string(3, ws) << "\"status\": {" << endl;
	
	cout << string(4, ws) << "\"workers\": {" << endl;
	cout << string(5, ws) << "\"minerals\": " << rm.getMineralWorkers() << "," << endl;
	cout << string(5, ws) << "\"vespene\": " << rm.getVespeneWorkers() << endl;
	cout << string(4, ws) << "}," << endl;
	
	cout << string(4, ws) << "\"resources\": {" << endl;
	cout << string(5, ws) << "\"minerals\": " << rm.getMinerals() << "," << endl;
	cout << string(5, ws) << "\"vespene\": " << rm.getVespene() << "," << endl;
	cout << string(5, ws) << "\"supply-used\": " << rm.getSupplyInt() << "," << endl;
	cout << string(5, ws) << "\"supply\": " << rm.getSupplyMaxInt() << endl;
	cout << string(4, ws) << "}" << endl;
	
	cout << string(3, ws) << "}," << endl;
	// events
	cout << string(3, ws) << "\"events\": [" << endl;
	for(auto& event : events) {
		if(!event->valid()) {
			std::cerr << "invalid event in jsonlogger::printMessage" << std::endl;
			continue;
		}
		cout << string(4, ws) << "{" << endl;
		cout << string(5, ws) << "\"type\": \"" << event->first() << "\"," << endl;
		cout << string(5, ws) << "\"name\": \"" << event->second() << "\"";// << endl;
		if (event->isAbilityEntry()) { //ability entry: print triggeredBy and targetBuilding
			cout << "," << endl;
			cout << string(5, ws) << "\"triggeredBy\": \"" << event->getID() << "\"," << endl;
			if(!event->getTarget().empty()) // terran doesn't need to specify a target
				cout << string(5, ws) << "\"targetBuilding\": \"" << event->getTarget() << "\"" << endl;
		} else {
			string producerID = event->getProducerID();
			vector<string> producedIDs = event->getProducedIDs();
			if (!producerID.empty()) {
				cout << "," << endl;
				cout << string(5, ws) << "\"producerID\": \"" << producerID << "\"";// << endl;
			}
			if (!producedIDs.empty()) {
				cout << "," << endl;
				cout << string(5, ws) << "\"producedIDs\": [" << endl;
				for (const string& s : producedIDs) {
					cout << string(6, ws) << "\"" << s << "\"" << (s == producedIDs.back() ? "" : ",") << endl;
				}
				cout << string(5, ws) << "]";
			}
		}
		cout << endl << string(4, ws) << "}" << (&event != &events.back() ? "," : "") << endl;
	}
	cout << string(3, ws) << "]" << endl;
	cout << string(2, ws) << "}";
	undo_redirect();
}

void JsonLogger::printMessageStart() {
	// add messages keyword at the beginning
	if(validBuildlist) {
		redirect();
		cout << ws << "\"messages\": [" << endl;
		undo_redirect();
	}
}
