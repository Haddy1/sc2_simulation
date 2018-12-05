#pragma once

enum Race {
	NONE = 0,
	TERRAN,
	PROTOSS,
	ZERG
};

inline const char* toString(Race r) {
	switch (r) {
			case TERRAN: return "sc2-hots-terran";
			case PROTOSS: return "sc2-hots-protoss";
			case ZERG: return "sc2-hots-zerg";
			default: return "";
	}
}
