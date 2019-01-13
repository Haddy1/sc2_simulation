#include "TerranBuilding.h"

unordered_map<string, vector<shared_ptr<TerranBuilding>>> TerranBuilding::buildingList=  
                                {{"refinery", vector<shared_ptr<TerranBuilding>>()}
                                ,{"engineering_bay", vector<shared_ptr<TerranBuilding>>()}
                                ,{"missile_tower", vector<shared_ptr<TerranBuilding>>()}
                                ,{"sensor_tower", vector<shared_ptr<TerranBuilding>>()}
                                ,{"armory", vector<shared_ptr<TerranBuilding>>()}
                                ,{"bunker", vector<shared_ptr<TerranBuilding>>()}
                                ,{"ghost_academy", vector<shared_ptr<TerranBuilding>>()}
                                ,{"fusion_core", vector<shared_ptr<TerranBuilding>>()}
                                ,{"supply_depot", vector<shared_ptr<TerranBuilding>>()}
};

unordered_map<string, vector<shared_ptr<FactoryBuilding>>> FactoryBuilding::factoryList =  
                {{"barracks", vector<shared_ptr<FactoryBuilding>>()}
                ,{"factory", vector<shared_ptr<FactoryBuilding>>()}
                ,{"starport", vector<shared_ptr<FactoryBuilding>>()}
                ,{"barracks_with_reactor", vector<shared_ptr<FactoryBuilding>>()}
                ,{"barracks_with_tech_lab", vector<shared_ptr<FactoryBuilding>>()}
                ,{"factory_with_reactor", vector<shared_ptr<FactoryBuilding>>()}
                ,{"factory_with_tech_lab", vector<shared_ptr<FactoryBuilding>>()}
                ,{"starport_with_reactor", vector<shared_ptr<FactoryBuilding>>()}
                ,{"starport_with_tech_lab", vector<shared_ptr<FactoryBuilding>>()}
};

vector<CommandCenter> CommandCenter::cCenterList = vector<CommandCenter>();
vector<string> CommandCenter::upgrades = {"command_center", "orbital_command", "planetary_fortress"};
vector<MULE> MULE::muleList = vector<MULE>();

TerranBuilding::TerranBuilding(string name, ResourceManager* resourceManager, vector<EventEntry>* eventList, SCV* constrWorker):
    Building(name)
    , name_(name)
    , rm(resourceManager)
    , eventList_(eventList)
    , constrWorker_(constrWorker)
    , underConstruction(true)
    , constrTimeRemaining(entityDataMap.at(name).buildTime)
{}

TerranBuilding::TerranBuilding(string name, ResourceManager* resourceManager, vector<EventEntry>* eventList):
    Building(name)
    , name_(name)
    , rm(resourceManager)
    , eventList_(eventList)
    , constrWorker_(nullptr)
    , underConstruction(false)
    , constrTimeRemaining(0)
{}

TerranBuilding::~TerranBuilding(){}

void TerranBuilding::update(){
    if (underConstruction)
    {
        if (constrTimeRemaining > 0){
            --constrTimeRemaining;
        }
        else {
            underConstruction = false;
            constrWorker_->busy = false;
            eventList_->emplace_back("build-end", name_,  constrWorker_->getID(), id);
            rm->addSupplyMax(getEntityData()->supplyProvided);
        }
    }
}

bool TerranBuilding::busy(){
    if (underConstruction) return true;
    else return false;
}

FactoryBuilding::FactoryBuilding(string name, ResourceManager* resourceManager, vector<EventEntry>* eventList, SCV* constrWorker):
    TerranBuilding(name, resourceManager, eventList, constrWorker)
    , rm(resourceManager)
{}

FactoryBuilding::FactoryBuilding(string name, ResourceManager* resourceManager, vector<EventEntry>* eventList):
    TerranBuilding(name, resourceManager, eventList)
    , rm(resourceManager)
{}

FactoryBuilding::~FactoryBuilding(){}

void FactoryBuilding::update(){
    if (underConstruction)
    {
        if (constrTimeRemaining > 0){
            --constrTimeRemaining;
        }
        else {
            underConstruction = false;
            constrWorker_->busy = false;
            if (addon_ == noAddon){
                eventList_->emplace_back("build-end", name_,  constrWorker_->getID(), id);
            }
            else {
                eventList_->emplace_back("build-end", name_,  id, id);
            }
        }
    }
    else if (workTimeRemaining > 0)
    {
        --workTimeRemaining;
    }
    else if (workTimeRemaining == 0)
    {
        TerranUnit newUnit(workName);
        TerranUnit::unitList.push_back(newUnit);
        rm->consumeSupply(newUnit.getEntityData()->supplyCost);
        eventList_->emplace_back("build-end", workName, id, getID());
        workTimeRemaining = -1;
    }
    if (addon_ == reactor){
        if (workTimeRemaining_reactor > 0){
            --workTimeRemaining_reactor;
        }
        else if (workTimeRemaining_reactor == 0){
            TerranUnit newUnit(workName_reactor);
            TerranUnit::unitList.push_back(newUnit);
            rm->consumeSupply(newUnit.getEntityData()->supplyCost);
            eventList_->emplace_back("build-end", workName_reactor, id, newUnit.getID());
            workTimeRemaining_reactor = -1;
        }
    }
}

bool FactoryBuilding::busy(){
    if (underConstruction) return true;
    if (workTimeRemaining > 0 && workTimeRemaining_reactor > 0) return true;
    return false;
}

bool FactoryBuilding::createUnit(string unitName){
    bool creationStarted = false;
    if (busy())
        creationStarted = false;
    else
    {
        EntityData unit = entityDataMap.at(unitName);
        if ( std::find(unit.producedBy.begin(), unit.producedBy.end(), name_) != unit.producedBy.end() && rm->canBuild(unit))
        {
            rm->consumeMinerals(unit.minerals);
            rm->consumeVespene(unit.vespene);

            if (workTimeRemaining == 0){
                workTimeRemaining = unit.buildTime;
            }
            else{
                workTimeRemaining_reactor = unit.buildTime;
            }
            creationStarted = true;
            eventList_->emplace_back("build-start", unitName, id);
        }
        else
        {
            creationStarted = false;
        }
    }
    return creationStarted;
}

bool FactoryBuilding::buildAddon(string addonName){
    if (addon_ != noAddon) return false;
    if (busy()) return false;
    string newName;
    AddonType addon;
    if (addonName.find("reactor") != string::npos){
        newName = name_ + "_with_reactor";
        addon = reactor;
    }
    else{
        newName = name_ + "_with_tech_lab";
        addon = tech_lab;
    }
    
    EntityData addonEntity = entityDataMap.at(newName);
    if (rm->canBuild(addonEntity)){
        rm->consumeMinerals(addonEntity.minerals);
        rm->consumeMinerals(addonEntity.vespene);
        name_ = newName;
        addon_ = addon;
        underConstruction = true;
        constrTimeRemaining = addonEntity.buildTime;
        factoryList.at(newName).push_back(shared_ptr<FactoryBuilding>(this));
        eventList_->emplace_back("build-start", addonName, id);
        return true;
    }

    return false;

}

CommandCenter::CommandCenter(string name, ResourceManager* resourceManager, vector<EventEntry>* eventList, SCV* constrWorker):
    TerranBuilding(name, resourceManager, eventList, constrWorker)
    ,rm(resourceManager) {}
CommandCenter::CommandCenter(string name, ResourceManager* resourceManager, vector<EventEntry>* eventList):
    TerranBuilding(name, resourceManager, eventList)
    ,rm(resourceManager) {}

CommandCenter::~CommandCenter() {
}

bool CommandCenter::busy(){
    if (constrTimeRemaining > 0 || workTimeRemaining > 0) return true;
    else return false;
}

void CommandCenter::update(){
    if (underConstruction)
    {
        if (constrTimeRemaining > 0)
            --constrTimeRemaining;
        else 
        {
            underConstruction = false;
            constrWorker_->busy = false;
            if (name_ == "command_center"){
                eventList_->emplace_back("build-end", name_, constrWorker_->getID(), id);
                rm->addSupplyMax(getEntityData()->supplyProvided);
            }
            else
                eventList_->emplace_back("build-end", name_, id, id);
        }
    }
    else if (workTimeRemaining > 0)
    {
        --workTimeRemaining;
    }
    else if (workTimeRemaining == 0)
    {
        SCV newWorker("scv", eventList_);
        SCV::workerList.push_back(newWorker);
        rm->consumeSupply(newWorker.getEntityData()->supplyCost);
        eventList_->emplace_back("build-end", "scv", id, newWorker.getID());
        workTimeRemaining = -1;
    }

    if (energy < maxEnergy){
        energy += energyRegen;
        if (energy > maxEnergy){
            energy = maxEnergy;
        }
    }

}

bool CommandCenter::createUnit(string unitName){
    bool creationStarted = false;
    if (busy())
        creationStarted = false;
    else
    {
        EntityData unit = entityDataMap.at(unitName);
        if (unitName == "scv" && rm->canBuild(unit))
        {
            rm->consumeMinerals(unit.minerals);
            rm->consumeVespene(unit.vespene);
            workTimeRemaining = unit.buildTime;
            creationStarted = true;
            eventList_->emplace_back("build-start", unitName, id);
        }
        else
        {
            creationStarted = false;
        }
    }
    return creationStarted;
}

bool CommandCenter::upgrade(string addonName){
    if (name_ != "command_center") return false;
    if (busy()) return false;
    string newName;
    if (addonName == "orbital_command")
        newName = "orbital_command";
    else
        newName = "planetary_fortress";

    EntityData upgradeEntity = entityDataMap.at(newName);
    if (rm->canBuild(upgradeEntity)){
        rm->consumeMinerals(upgradeEntity.minerals);
        rm->consumeVespene(upgradeEntity.vespene);

        name_ = newName;
        underConstruction = true;
        constrTimeRemaining = upgradeEntity.buildTime;

        eventList_->emplace_back("build-start", addonName, id);
        return true;
    }
    return false;
    
}

bool CommandCenter::callMule(){
    bool success = false;
    if (name_ == "orbital_command" && energy >= MULE::energyCost ){
        MULE newMule(rm, eventList_);
        MULE::muleList.push_back(newMule);
        success = true;
        eventList_->emplace_back("special", "mule", id);
    }

    return success;
}

