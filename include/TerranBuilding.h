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
struct TerranUnits;


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
    SCV* constrWorker_;
    bool underConstruction;
    int constrTimeRemaining;
    Tech* tech_;

public:
    TerranBuilding(int& ID_Counter, EntityType buildType, ResourceManager* resourceManager, Tech* tech, JsonLoggerV2* eventList, SCV* constrWorker);
    TerranBuilding(int& ID_Counter, EntityType buildType, ResourceManager* resourceManager, Tech* tech, JsonLoggerV2* eventList);
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

    FactoryBuilding(int& ID_Counter, EntityType buildType, ResourceManager* resourceManager, Tech* tech, TerranUnits* units, JsonLoggerV2* eventList, SCV* constrWorker);
    FactoryBuilding(int& ID_Counter, EntityType buildType, ResourceManager* resourceManager, Tech* tech, TerranUnits* units, JsonLoggerV2* eventList);
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
        TerranUnits* units_;

        

};

class CommandCenter : public TerranBuilding
{
public:
    static const vector<EntityType> upgrades;

    CommandCenter(int& ID_Counter, EntityType buildType, ResourceManager* resourceManager, Tech* tech, TerranUnits* units, JsonLoggerV2* eventList, SCV* constrWorker);
    CommandCenter(int& ID_Counter, EntityType buildType, ResourceManager* resourceManager, Tech* tech, TerranUnits* units, JsonLoggerV2* eventList);
    ~CommandCenter();
    bool busy();
    bool createUnit(EntityType unitType);
    void update(int& ID_Counter);
    bool upgrade(EntityType upgrade);

private:
    ResourceManager* rm;
    TerranUnits* units_;
    int workTimeRemaining = 0;
    bool producing = false;
    FixedPoint energy = 0;
    const FixedPoint energyRegen = 0.625;
    int muleLifetime = -1;
    const static int muleEnergyCost = 50;
    const int muleInitLifetime = 90;
};

struct TerranBuildings{
    unordered_map<EntityType, vector<shared_ptr<TerranBuilding>>> buildingList =  
                                {{refinery, vector<shared_ptr<TerranBuilding>>()}
                                ,{engineering_bay, vector<shared_ptr<TerranBuilding>>()}
                                ,{missile_turret, vector<shared_ptr<TerranBuilding>>()}
                                ,{sensor_tower, vector<shared_ptr<TerranBuilding>>()}
                                ,{armory, vector<shared_ptr<TerranBuilding>>()}
                                ,{bunker, vector<shared_ptr<TerranBuilding>>()}
                                ,{ghost_academy, vector<shared_ptr<TerranBuilding>>()}
                                ,{fusion_core, vector<shared_ptr<TerranBuilding>>()}
                                ,{supply_depot, vector<shared_ptr<TerranBuilding>>()}
                                };
    unordered_map<EntityType, vector<shared_ptr<FactoryBuilding>>> factoryList =  
            {{barracks, vector<shared_ptr<FactoryBuilding>>()}
            ,{factory, vector<shared_ptr<FactoryBuilding>>()}
            ,{starport, vector<shared_ptr<FactoryBuilding>>()}
            };

    vector<CommandCenter> cCenterList = vector<CommandCenter>();
    void clear(){
            for (auto& entry : buildingList){
                entry.second.clear();
            }
            for (auto& entry : factoryList){
                entry.second.clear();
            }
            cCenterList.clear();
    }
};
