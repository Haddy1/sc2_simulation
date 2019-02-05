#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "Building.h"
#include "EntityData.h"
#include "ResourceManager.h"
#include "Tech.h"
#include <algorithm>
#include <memory>
#include "JsonLoggerV2.h"
#include "TerranUnit.h"

class SCV;
class TerranUnits;


using std::string;
using std::unordered_map;
using std::vector;
using std::shared_ptr;

class TerranBuilding : public Building
{
protected:
    EntityType buildType_;
    ResourceManager* rm;
    JsonLoggerV2* logger_;
    int constrWorkerID_;
    bool underConstruction;
    int constrTimeRemaining;
    Tech* tech_;
    TerranUnits* units_;
    bool logging_;

public:
    TerranBuilding(int& ID_Counter, EntityType buildType, ResourceManager* resourceManager, Tech* tech, TerranUnits* units, JsonLoggerV2* eventList, int constrWorkerID, bool logging);
    TerranBuilding(int& ID_Counter, EntityType buildType, ResourceManager* resourceManager, Tech* tech, TerranUnits* units, JsonLoggerV2* eventList, bool logging);
    ~TerranBuilding();
    void update();
    bool busy();
    bool isUnderConstruction() {return underConstruction;}
};


class FactoryBuilding : public TerranBuilding{
    public: 
    enum AddonType{
        noAddon = 0
        ,reactor = 1
        ,tech_lab = 2
    };
    static const unordered_map<EntityType,EntityType> labBaseBuildings;
    static const unordered_map<EntityType,EntityType> reactorBaseBuildings;

    FactoryBuilding(int& ID_Counter, EntityType buildType, ResourceManager* resourceManager, Tech* tech, TerranUnits* units, JsonLoggerV2* eventList, int constrWorkerID, bool logging);
    FactoryBuilding(int& ID_Counter, EntityType buildType, ResourceManager* resourceManager, Tech* tech, TerranUnits* units, JsonLoggerV2* eventList, bool logging);
    ~FactoryBuilding();
    bool createUnit(EntityType unitType);
    bool buildAddon(EntityType addon, AddonType addonType);
    bool busy();
    void update(int& ID_Counter);
    EntityType getAddon(){return addon_;};
    private:
        ResourceManager* rm;
        EntityType workType = none;
        EntityType workTypeReactor = none;
        int workTimeRemaining = 0;
        int workTimeRemaining_reactor = 0;
        EntityType addon_ = none;
        AddonType addonType_ = noAddon;
        bool producing = false;
        bool producing_reactor = false;

        

};

class CommandCenter : public TerranBuilding
{
public:
    static const vector<EntityType> upgrades;

    CommandCenter(int& ID_Counter, EntityType buildType, ResourceManager* resourceManager, Tech* tech, TerranUnits* units, JsonLoggerV2* eventList, int constrWorkerID, bool logging);
    CommandCenter(int& ID_Counter, EntityType buildType, ResourceManager* resourceManager, Tech* tech, TerranUnits* units, JsonLoggerV2* eventList, bool logging);
    ~CommandCenter();
    bool busy();
    bool createUnit(EntityType unitType);
    void update(int& ID_Counter);
    bool upgrade(EntityType upgrade);
    bool callMule();

private:
    ResourceManager* rm;
    int workTimeRemaining = 0;
    bool producing = false;
    FixedPoint energy = 0;
    const FixedPoint energyRegen = 0.625;
    int muleLifetime = -1;
    const static int muleEnergyCost = 50;
    const int muleInitLifetime = 90;
};

class TerranBuildings{
    public:
    vector<TerranBuilding> buildingList ;
    unordered_map<EntityType, vector<FactoryBuilding>> factoryList;
    vector<CommandCenter> cCenterList;
    TerranBuildings():
    buildingList() 

    , factoryList(  
            {{barracks, vector<FactoryBuilding>()}
            ,{factory, vector<FactoryBuilding>()}
            ,{starport, vector<FactoryBuilding>()}
            })
    ,cCenterList( vector<CommandCenter>())
    {}

};
