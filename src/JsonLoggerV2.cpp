#include "../include/JsonLoggerV2.h"

// log into cout
JsonLoggerV2::JsonLoggerV2(Race r, ResourceManager& manager, bool valid) : race(r), rm(manager), validBuildlist(valid), hasBuildstart(false), hasAbility(false), messages(0) {
	
}

JsonLoggerV2::~JsonLoggerV2() {
	
}

void JsonLoggerV2::printBeginning() {
	cout << '{' << endl; 
	cout << ws << quote << "buildlistValid" << quote << ": " << (int) validBuildlist << ',' << endl; 
	cout << ws << quote << "game" << quote << ": " << quote << toString(race) << quote << (validBuildlist ? "," : "") << endl;
}

void JsonLoggerV2::printEnd() {
	if (validBuildlist) {
		cout << endl << ws << ']' << endl; // ends the "messages" block
	}
	cout << '}' << endl;
}

void JsonLoggerV2::printSetup(vector<pair<string, vector<int>>>& units) {
	// print initial units
	if(validBuildlist) {
		cout << ws << quote << "initialUnits" << quote << " : {" << endl;
		for(auto& unit : units) {
			cout << string(2, ws) << quote << unit.first << quote << " : [" << endl;
			for(auto& id : unit.second) {
				cout << string(3, ws) << quote << id << quote;
				if (id != unit.second.back()) {
					cout << ',' << endl;
				} else {
					cout << endl << string(2, ws) << ']';
				}
			}
			cout << (unit != units.back() ? ", " : "") << endl;
		}
		cout << ws << "}," << endl;
		cout << ws << "\"messages\": [" << endl;
	}
}

void JsonLoggerV2::addBuildstart(BuildStartEntry b) {
	buildstartEvent = b;
	hasBuildstart = true;
}

void JsonLoggerV2::addBuildend(BuildEndEntry b) {
	buildendEvents.push_back(b);
}

void JsonLoggerV2::addSpecial(SpecialEntry a) {
	abilityEvent = a;
	hasAbility = true;
}

void JsonLoggerV2::printMessage(int time) {
	// messages not added to log for invalid build lists
	if(!validBuildlist) {
		return;
	}
	int num = 0;
	int count = 0;
	if (hasBuildstart) {
		++num;
	}
	num += buildendEvents.size();
	if (hasAbility) {
		++num;
	}
	if (num == 0) {
		return;
	}
	if (messages != 0) {
		cout << "," << endl; // separation with previous message entry
	}
	cout << string(2, ws) << "{" << endl;
	cout << string(3, ws) << "\"time\": " << time << "," << endl;
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
	
	if (hasBuildstart) {
		++count;
		cout << string(4, ws) << "{" << endl;
		cout << string(5, ws) << "\"type\": \"" << "build-start" << "\"," << endl;
		cout << string(5, ws) << "\"name\": \"" << buildstartEvent.name << quote;
		if (buildstartEvent.producerID.size() > 0) {
			cout << ',' << endl;
			cout << string(5, ws) << "\"producerID\": \"" << buildstartEvent.producerID << quote << endl;
		} else {
			cout << endl;
		}
		
				
		cout << string(4, ws) << "}" << (count < num ? "," : "") << endl;
	}
	
	for (auto& entry : buildendEvents) {
		++count;
		cout << string(4, ws) << "{" << endl;
		cout << string(5, ws) << "\"type\": \"" << "build-end" << "\"," << endl;
		cout << string(5, ws) << "\"name\": \"" << entry.name << quote;
		if (entry.producerID.size() > 0) {
			cout << ',' << endl;
			cout << string(5, ws) << "\"producerID\": \"" << entry.producerID << quote;
		} else {
			cout << endl;
		}
		if (entry.producedIDs.size() > 0) {
			cout << "," << endl;
			cout << string(5, ws) << "\"producedIDs\": [" << endl;
			for (const string& s : entry.producedIDs) {
				cout << string(6, ws) << quote << s << quote << (s == entry.producedIDs.back() ? "" : ",") << endl;
			}
			cout << string(5, ws) << "]" << endl;
			
		} else {
			cout << endl;
		}
		
		cout << string(4, ws) << "}" << (count < num ? "," : "") << endl;
	}
	
	if (hasAbility) {
		cout << string(4, ws) << "{" << endl;
		cout << string(5, ws) << "\"type\": \"" << "special" << quote << ',' << endl;
		cout << string(5, ws) << "\"name\": \"" << abilityEvent.name << quote << ',' << endl;
		cout << string(5, ws) << "\"triggeredBy\": \"" << abilityEvent.triggeredBy << quote;
		if (abilityEvent.targetBuilding.size() > 0) {
			cout << ',' << endl;
			cout << string(5, ws) << "\"targetBuilding\": \"" << abilityEvent.targetBuilding << quote << endl;
		} else {
			cout << endl;
		}
		
		cout << endl << string(4, ws) << "}" << endl;
	}
	
	
	cout << string(3, ws) << "]" << endl;
	cout << string(2, ws) << "}";
	
	
	
	hasBuildstart = false;
	buildendEvents.clear();
	hasAbility = false;
	
	++messages;
}
