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


using std::string;
using std::unordered_map;
using std::vector;
using std::shared_ptr;

class TerranBuilding : public Building
{
    public:
    static unordered_map<string,vector<shared_ptr<TerranBuilding>>> buildingList;

public:
protected:
    string name_;
    ResourceManager* rm;
    JsonLoggerV2* logger_;
    SCV* constrWorker_;
    bool underConstruction;
    int constrTimeRemaining;

public:
    TerranBuilding(int& ID_Counter, string name, ResourceManager* resourceManager, JsonLoggerV2* eventList, SCV* constrWorker);
    TerranBuilding(int& ID_Counter, string name, ResourceManager* resourceManager, JsonLoggerV2* eventList);
    ~TerranBuilding();
    void update();
    bool busy();
    string getName() {return name_; }
};

class FactoryBuilding : public TerranBuilding{
    enum AddonType{
        noAddon = 0
        ,reactor = 1
        ,tech_lab = 2
    };
    public: 
    static unordered_map<string,vector<shared_ptr<FactoryBuilding>>> factoryList;

    FactoryBuilding(int& ID_Counter, string name, ResourceManager* resourceManager, JsonLoggerV2* eventList, SCV* constrWorker);
    FactoryBuilding(int& ID_Counter, string name, ResourceManager* resourceManager, JsonLoggerV2* eventList);
    ~FactoryBuilding();
    bool createUnit(string unitName);
    bool buildAddon(string addon);
    bool busy();
    void update(int& ID_Counter);
    private:
        ResourceManager* rm;
        string workName = "";
        string workName_reactor = "";
        int workTimeRemaining = 0;
        int workTimeRemaining_reactor = 0;
        AddonType addon_ = noAddon;
        bool producing = false;
        bool producing_reactor = false;

        

};

class CommandCenter : public TerranBuilding
{
public:
    static vector<CommandCenter> cCenterList;
    static vector<string> upgrades;

    CommandCenter(int& ID_Counter, string name, ResourceManager* resourceManager, JsonLoggerV2* eventList, SCV* constrWorker);
    CommandCenter(int& ID_Counter, string name, ResourceManager* resourceManager, JsonLoggerV2* eventList);
    ~CommandCenter();
    bool busy();
    bool createUnit(string unitName);
    bool callMule();
    void update(int& ID_Counter);
    bool upgrade(string upgradeName);

private:
    ResourceManager* rm;
    int workTimeRemaining = 0;
    bool producing = false;
    FixedPoint energy = 0;
    const FixedPoint energyRegen = 0.625;
    FixedPoint maxEnergy = 200;
    string upgradeConstructed = ""; // Stores the upgrade name 
};


