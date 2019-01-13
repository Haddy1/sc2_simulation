#include "TerranBuilding.h"

unordered_map<string, vector<shared_ptr<TerranBuilding>>> TerranBuilding::buildingList=  
                                {{"refinery", vector<shared_ptr<TerranBuilding>>()}
                                ,{"engineering_bay", vector<shared_ptr<TerranBuilding>>()}
                                ,{"missile_turret", vector<shared_ptr<TerranBuilding>>()}
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
//                ,{"barracks_with_reactor", vector<shared_ptr<FactoryBuilding>>()}
//                ,{"barracks_with_tech_lab", vector<shared_ptr<FactoryBuilding>>()}
//                ,{"factory_with_reactor", vector<shared_ptr<FactoryBuilding>>()}
//                ,{"factory_with_tech_lab", vector<shared_ptr<FactoryBuilding>>()}
//                ,{"starport_with_reactor", vector<shared_ptr<FactoryBuilding>>()}
//                ,{"starport_with_tech_lab", vector<shared_ptr<FactoryBuilding>>()}
};

vector<CommandCenter> CommandCenter::cCenterList = vector<CommandCenter>();
vector<string> CommandCenter::upgrades = {"command_center", "orbital_command", "planetary_fortress"};
vector<MULE> MULE::muleList = vector<MULE>();

TerranBuilding::TerranBuilding(int& ID_Counter, string name, ResourceManager* resourceManager, JsonLoggerV2* eventList, SCV* constrWorker):
    Building(ID_Counter, name)
    , name_(name)
    , rm(resourceManager)
    , logger_(eventList)
    , constrWorker_(constrWorker)
    , underConstruction(true)
    , constrTimeRemaining(entityDataMap.at(name).buildTime)
{}

TerranBuilding::TerranBuilding(int& ID_Counter, string name, ResourceManager* resourceManager, JsonLoggerV2* eventList):
    Building(ID_Counter, name)
    , name_(name)
    , rm(resourceManager)
    , logger_(eventList)
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
        if (constrTimeRemaining == 0){
            underConstruction = false;
            constrWorker_->busy = false;
            logger_->addBuildend(BuildEndEntry(name_, constrWorker_->getID(), id));
            rm->addSupplyMax(getEntityData()->supplyProvided);
            techAdd(name_);
            if (name_ == "refinery")
                rm->incrementGeysers();
        }
    }
}

bool TerranBuilding::busy(){
    if (underConstruction) return true;
    else return false;
}

FactoryBuilding::FactoryBuilding(int& ID_Counter, string name, ResourceManager* resourceManager, JsonLoggerV2* eventList, SCV* constrWorker):
    TerranBuilding(ID_Counter, name, resourceManager, eventList, constrWorker)
    , rm(resourceManager)
{}

FactoryBuilding::FactoryBuilding(int& ID_Counter, string name, ResourceManager* resourceManager, JsonLoggerV2* eventList):
    TerranBuilding(ID_Counter, name, resourceManager, eventList)
    , rm(resourceManager)
{}

FactoryBuilding::~FactoryBuilding(){}

void FactoryBuilding::update(int& ID_Counter){
    if (underConstruction)
    {
        if (constrTimeRemaining > 0){
            --constrTimeRemaining;
        }
        if (constrTimeRemaining == 0){
            underConstruction = false;
            constrWorker_->busy = false;
            if (addon_ == noAddon){
                logger_->addBuildend(BuildEndEntry(name_, constrWorker_->getID(), id));
                techAdd(name_);
            }
            else {
                logger_->addBuildend(BuildEndEntry(name_, id, id));
                techAdd(addonName_);
            }
        }
    }
    else if(producing){
    if (workTimeRemaining > 0)
    {
        --workTimeRemaining;
    }
    if (workTimeRemaining == 0)
    {
        TerranUnit newUnit(ID_Counter, workName);
        TerranUnit::unitList.push_back(newUnit);
        logger_->addBuildend(BuildEndEntry(workName, id, newUnit.getID()));
        workTimeRemaining = 0;
        producing = false;
    }
    }
    if (addon_ == reactor && producing_reactor){
        if (workTimeRemaining_reactor > 0){
            --workTimeRemaining_reactor;
        }
        if (workTimeRemaining_reactor == 0){
            TerranUnit newUnit(ID_Counter, workName_reactor);
            TerranUnit::unitList.push_back(newUnit);
            logger_->addBuildend(BuildEndEntry(workName_reactor, id, newUnit.getID()));
            workTimeRemaining_reactor = 0;
            producing_reactor = false;
        }
    }
}

bool FactoryBuilding::busy(){
    if (underConstruction) return true;
    if (workTimeRemaining > 0 ){
        if (addon_ == noAddon) return true;
        else if (workTimeRemaining_reactor > 0) return true;
    }

    return false;
}

bool FactoryBuilding::createUnit(string unitName){
    if (busy())
        return false;
    else
    {
        EntityData unit = entityDataMap.at(unitName);
                if(rm->canBuild(unit)){
        if ( std::find(unit.producedBy.begin(), unit.producedBy.end(), name_) != unit.producedBy.end() || std::find(unit.producedBy.begin(), unit.producedBy.end(), addonName_) != unit.producedBy.end())
        {
            rm->consumeMinerals(unit.minerals);
            rm->consumeVespene(unit.vespene);
            rm->consumeSupply(unit.supplyCost);

            if (! producing){
                workName = unitName;
                workTimeRemaining = unit.buildTime;
                producing = true;
            }
            else if (addon_ == reactor and !producing_reactor) {
                workName_reactor = unitName;
                workTimeRemaining_reactor = unit.buildTime;
                producing_reactor = true;
            }
            logger_->addBuildstart(BuildStartEntry(unitName, id));
            return true;
        }
                }
        else
        {
            return false;
        }
    }
    return false;
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

        logger_->addBuildstart(BuildStartEntry(addonName, id));
        return true;
    }

    return false;

}

CommandCenter::CommandCenter(int& ID_Counter, string name, ResourceManager* resourceManager, JsonLoggerV2* eventList, SCV* constrWorker):
    TerranBuilding(ID_Counter, name, resourceManager, eventList, constrWorker)
    ,rm(resourceManager) {}
CommandCenter::CommandCenter(int& ID_Counter, string name, ResourceManager* resourceManager, JsonLoggerV2* eventList):
    TerranBuilding(ID_Counter, name, resourceManager, eventList)
    ,rm(resourceManager) {}

CommandCenter::~CommandCenter() {
}

bool CommandCenter::busy(){
    if (underConstruction || producing) return true;
    else return false;
}

void CommandCenter::update(int& ID_Counter){
    if (!underConstruction && upgradeConstructed == "orbital_command" && energy < maxEnergy){
        energy += energyRegen;
        if (energy > maxEnergy){
            energy = maxEnergy;
        }
    }
    if (underConstruction)
    {
        if (constrTimeRemaining > 0)
            --constrTimeRemaining;
        if (constrTimeRemaining == 0){
            underConstruction = false;
            if (upgradeConstructed.empty()){
                constrWorker_->busy = false;
                logger_->addBuildend(BuildEndEntry(name_, constrWorker_->getID(), id));
                rm->addSupplyMax(getEntityData()->supplyProvided);
                techAdd("command_center");
            }
            else{
                energy = entityDataMap.at("orbital_command").startEnergy;
                logger_->addBuildend(BuildEndEntry(upgradeConstructed, id, id));
                name_ = upgradeConstructed;
                techAdd(upgradeConstructed);
            }
        }
    }
    else if (producing){
        if (workTimeRemaining > 0)
        {
            --workTimeRemaining;
        }
        if (workTimeRemaining == 0)
        {
            SCV newWorker(ID_Counter, "scv", logger_);
            SCV::workerList.push_back(newWorker);
            logger_->addBuildend(BuildEndEntry("scv", id, newWorker.getID()));
            producing = false;
        }
    }


}

bool CommandCenter::createUnit(string unitName){
    if (busy()) 
        return false;
    else
    {
        EntityData unit = entityDataMap.at(unitName);
        if (rm->canBuild(unit))
        {
            rm->consumeMinerals(unit.minerals);
            rm->consumeVespene(unit.vespene);
            rm->consumeSupply(unit.supplyCost);
            workTimeRemaining = unit.buildTime;
            producing = true;
            logger_->addBuildstart(BuildStartEntry(unitName, id));
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

bool CommandCenter::upgrade(string addonName){
    if (! upgradeConstructed.empty()) return false;
    if (busy()) return false;
    
    EntityData upgradeEntity = entityDataMap.at(addonName);
    if (rm->canBuild(upgradeEntity)){
        rm->consumeMinerals(upgradeEntity.minerals);
        rm->consumeVespene(upgradeEntity.vespene);
        
        upgradeConstructed = addonName;
        underConstruction = true;
        constrTimeRemaining = upgradeEntity.buildTime;

        logger_->addBuildstart(BuildStartEntry(addonName, id));
        return true;
    }
    return false;
    
}

bool CommandCenter::callMule(){
    if (upgradeConstructed == "orbital_command" && !underConstruction && energy >= MULE::energyCost ){
        energy -= MULE::energyCost;
        MULE newMule(rm, logger_);
        MULE::muleList.push_back(newMule);
        logger_->addSpecial(SpecialEntry("mule", id));
        return true;
    }

    return false;
}

