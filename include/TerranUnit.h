#pragma once 

#include <vector>
#include <string>
#include <memory>
#include <assert.h>
#include <algorithm>
#include "TerranBuilding.h"
#include "ResourceManager.h"
#include "Tech.h"
#include "Unit.h"
#include "JsonLoggerV2.h"

using std::vector;
using std::string;
using std::shared_ptr;

class TerranBuildings;
class TerranUnits;

class TerranUnit : public Unit
{
    bool constructing = false;
protected:

public:
    TerranUnit(int& ID_Counter, EntityType unitType);
    ~TerranUnit();
    static void clearList();
};

class SCV : public TerranUnit
{
    JsonLoggerV2* logger_;
    bool logging_;
public:
    SCV(int& ID_Counter, EntityType unitType, JsonLoggerV2* eventList, bool logging);
    ~SCV();
    bool construct(int& ID_Counter, EntityType buildType, ResourceManager* rm, Tech* tech, TerranBuildings& buildings, TerranUnits* units);
    bool busy = false;
};

// Container Class for Terran Units
class TerranUnits{
    public:
        unordered_map<int, SCV> workerList;
        unordered_map<EntityType, vector<TerranUnit>> unitList;
        TerranUnits():
        workerList(unordered_map<int, SCV>())
    ,unitList(
            {{marine, vector<TerranUnit>()}
            ,{marauder, vector<TerranUnit>()}
            ,{reaper, vector<TerranUnit>()}
            ,{ghost, vector<TerranUnit>()}
            ,{hellion, vector<TerranUnit>()}
            ,{siege_tank, vector<TerranUnit>()}
            ,{thor, vector<TerranUnit>()}
            ,{medivac, vector<TerranUnit>()}
            ,{viking, vector<TerranUnit>()}
            ,{raven, vector<TerranUnit>()}
            ,{banshee, vector<TerranUnit>()}
            ,{battlecruiser, vector<TerranUnit>()}
            })
        {}

    void clear(){
            for (auto& entry : unitList){
                entry.second.clear();
            }
    }
};
