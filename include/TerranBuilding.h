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
#include "JsonLogger.h"
#include "TerranUnit.h"

class SCV;


using std::string;
using std::unordered_map;
using std::vector;
using std::shared_ptr;

class TerranBuilding
{
    public:
    static unordered_map<string,vector<shared_ptr<TerranBuilding>>> buildingList;
    vector<string> techLabs = {"barracks_with_reactor", "factory_with_reactor", "starport_with_reactor"};

public:
protected:
    string name_;
    string id;
    bool underConstruction;
    int constrTimeRemaining;
    SCV* constrWorker_;
    vector<EventEntry>* eventList_;

public:
    TerranBuilding(string name, vector<EventEntry>* eventList, SCV* constrWorker);
    TerranBuilding(string name, vector<EventEntry>* eventList);
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

    FactoryBuilding(string name, vector<EventEntry>* eventList, ResourceManager* resourceManager, SCV* constrWorker);
    FactoryBuilding(string name, vector<EventEntry>* eventList, ResourceManager* resourceManager);
    ~FactoryBuilding();
    bool createUnit(string unitName);
    bool buildAddon(string addon);
    bool busy();
    void update();
    private:
        ResourceManager* rm;
        string workName = "";
        string workName_reactor = "";
        int workTimeRemaining = 0;
        int workTimeRemaining_reactor = 0;
        AddonType addon_ = noAddon;
        string addonID = "";
        

};

class CommandCenter : public TerranBuilding
{
public:
    static vector<CommandCenter> cCenterList;
    static vector<string> upgrades;

    CommandCenter(string name, vector<EventEntry>* eventList, ResourceManager* resourceManager, SCV* constrWorker);
    CommandCenter(string name, vector<EventEntry>* eventList, ResourceManager* resourceManager);
    ~CommandCenter();
    bool busy();
    bool createUnit(string unitName);
    bool callMule();
    void update();
    bool upgrade(string upgradeName);

private:
    ResourceManager* rm;
    int workTimeRemaining = 0;
    FixedPoint energy = 0;
    const FixedPoint energyRegen = 0.625;
    FixedPoint maxEnergy = 200;
};


