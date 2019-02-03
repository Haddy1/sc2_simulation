#include "TerranBuilding.h"



const unordered_map<EntityType,EntityType> FactoryBuilding::labBaseBuildings = {{barracks_with_tech_lab, barracks}, {factory_with_tech_lab, factory}, {starport_with_tech_lab, starport}};

const unordered_map<EntityType,EntityType> FactoryBuilding::reactorBaseBuildings = {{barracks_with_reactor, barracks}, {factory_with_reactor, factory}, {starport_with_reactor, starport}};

const vector<EntityType> CommandCenter::upgrades = {command_center, orbital_command, planetary_fortress};

TerranBuilding::TerranBuilding(int& ID_Counter, EntityType buildType, ResourceManager* resourceManager, Tech* tech, JsonLoggerV2* eventList, SCV* constrWorker):
    Building(ID_Counter, buildType)
    , rm(resourceManager)
    , logger_(eventList)
    , constrWorker_(constrWorker)
    , underConstruction(true)
    , constrTimeRemaining(entityDataMap.at(buildType).buildTime)
    , tech_(tech)
{}

TerranBuilding::TerranBuilding(int& ID_Counter, EntityType buildType, ResourceManager* resourceManager, Tech* tech, JsonLoggerV2* eventList):
    Building(ID_Counter, buildType)
    , rm(resourceManager)
    , logger_(eventList)
    , constrWorker_(nullptr)
    , underConstruction(false)
    , constrTimeRemaining(0)
    , tech_(tech)
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
            logger_->addBuildend(BuildEndEntry(getName(), constrWorker_->getID(), id));
            rm->addSupplyMax(getEntityData()->supplyProvided);
            tech_->add(getType());
            if (getType() == refinery)
                rm->incrementGeysers();
        }
    }
}

bool TerranBuilding::busy(){
    if (underConstruction) return true;
    else return false;
}

FactoryBuilding::FactoryBuilding(int& ID_Counter, EntityType buildType, ResourceManager* resourceManager, Tech* tech, TerranUnits* units, JsonLoggerV2* eventList, SCV* constrWorker):
    TerranBuilding(ID_Counter, buildType, resourceManager, tech, eventList, constrWorker)
    , rm(resourceManager)
    , units_(units)
{}

FactoryBuilding::FactoryBuilding(int& ID_Counter, EntityType buildType, ResourceManager* resourceManager, Tech* tech, TerranUnits* units, JsonLoggerV2* eventList):
    TerranBuilding(ID_Counter, buildType, resourceManager, tech, eventList)
    , rm(resourceManager)
    , units_(units)
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
            if (addonType_ == noAddon){
            	constrWorker_->busy = false;
                logger_->addBuildend(BuildEndEntry(getName(), constrWorker_->getID(), id));
                tech_->add(getType());
            }
            else {
                logger_->addBuildend(BuildEndEntry(entityNameMap.at(addon_), id, id));
                tech_->add(addon_);
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
        TerranUnit newUnit(ID_Counter, workType);
        units_->unitList.at(workType).push_back(newUnit);
        logger_->addBuildend(BuildEndEntry(newUnit.getName(), id, newUnit.getID()));
        workTimeRemaining = 0;
        producing = false;
    }
    }
    if (addonType_ == reactor && producing_reactor){
        if (workTimeRemaining_reactor > 0){
            --workTimeRemaining_reactor;
        }
        if (workTimeRemaining_reactor == 0){
            TerranUnit newUnit(ID_Counter, workTypeReactor);
            units_->unitList.at(workTypeReactor).push_back(newUnit);
            logger_->addBuildend(BuildEndEntry(newUnit.getName(), id, newUnit.getID()));
            workTimeRemaining_reactor = 0;
            producing_reactor = false;
        }
    }
}

bool FactoryBuilding::busy(){
    if (underConstruction) return true;
    if (workTimeRemaining > 0 ){
        if (addonType_ == noAddon) return true;
        else if (workTimeRemaining_reactor > 0) return true;
    }

    return false;
}

bool FactoryBuilding::createUnit(EntityType unitType){
    if (busy())
        return false;
    else
    {
        EntityData unit = entityDataMap.at(unitType);
                if(rm->canBuild(unit)){
        if ( std::find(unit.producedBy.begin(), unit.producedBy.end(), getType()) != unit.producedBy.end() || std::find(unit.producedBy.begin(), unit.producedBy.end(), addon_) != unit.producedBy.end())
        {
            rm->consumeMinerals(unit.minerals);
            rm->consumeVespene(unit.vespene);
            rm->consumeSupply(unit.supplyCost);

            if (! producing){
                workType = unitType;
                workTimeRemaining = unit.buildTime;
                producing = true;
            }
            else if (addonType_ == reactor and !producing_reactor) {
                workTypeReactor = unitType;
                workTimeRemaining_reactor = unit.buildTime;
                producing_reactor = true;
            }
            logger_->addBuildstart(BuildStartEntry(entityNameMap.at(unitType), id));
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

bool FactoryBuilding::buildAddon(EntityType addon, AddonType addonType){
    if (addonType_ != noAddon) return false;
    if (busy()) return false;
    
    EntityData addonEntity = entityDataMap.at(addon);
    if (rm->canBuild(addonEntity)){
        rm->consumeMinerals(addonEntity.minerals);
        rm->consumeVespene(addonEntity.vespene);
        addon_ = addon;
        addonType_ = addonType;
        underConstruction = true;
        constrTimeRemaining = addonEntity.buildTime;

        logger_->addBuildstart(BuildStartEntry(entityNameMap.at(addon), id));
        return true;
    }

    return false;

}

CommandCenter::CommandCenter(int& ID_Counter, EntityType buildType, ResourceManager* resourceManager, Tech* tech, TerranUnits* units, JsonLoggerV2* eventList, SCV* constrWorker):
    TerranBuilding(ID_Counter, buildType, resourceManager, tech, eventList, constrWorker)
    ,rm(resourceManager) 
    ,units_(units)
{}
CommandCenter::CommandCenter(int& ID_Counter, EntityType buildType, ResourceManager* resourceManager, Tech* tech, TerranUnits* units, JsonLoggerV2* eventList):
    TerranBuilding(ID_Counter, buildType, resourceManager, tech, eventList)
    ,rm(resourceManager)
    ,units_(units)
{}

CommandCenter::~CommandCenter() {
}

bool CommandCenter::busy(){
    if (underConstruction || producing) return true;
    else return false;
}

void CommandCenter::update(int& ID_Counter){
    if (!underConstruction && getType() == orbital_command ){
    	if (energy < entityData->maxEnergy){
            energy += energyRegen;
            if (energy > entityData->maxEnergy){
                energy = entityData->maxEnergy;
            }
    	}
        if (muleLifetime > 0){
            rm->addMinerals(rm->mineralsPerWorkerSecond);
            rm->addMinerals(rm->mineralsPerWorkerSecond);
            rm->addMinerals(rm->mineralsPerWorkerSecond);
            rm->addMinerals(rm->mineralsPerWorkerSecond);
            muleLifetime -= 1;
        }
        else if (energy >= muleEnergyCost and muleLifetime <= 0){
        	energy -= muleEnergyCost;
        	muleLifetime = muleInitLifetime;
        	logger_->addSpecial(SpecialEntry("mule", id));
        }
    }
    if (underConstruction)
    {
        if (constrTimeRemaining > 0)
            --constrTimeRemaining;
        if (constrTimeRemaining == 0){
            underConstruction = false;
            if (getType() == command_center){
                constrWorker_->busy = false;
                logger_->addBuildend(BuildEndEntry(getName(), constrWorker_->getID(), id));
                rm->addSupplyMax(getEntityData()->supplyProvided);
                tech_->add(command_center);
            }
            else{
                energy = entityDataMap.at(orbital_command).startEnergy;
                logger_->addBuildend(BuildEndEntry(getName(), id, id));
                tech_->add(getType());
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
            SCV newWorker(ID_Counter, scv, logger_);
            units_->workerList.push_back(newWorker);
            logger_->addBuildend(BuildEndEntry("scv", id, newWorker.getID()));
            producing = false;
        }
    }



}

bool CommandCenter::createUnit(EntityType unitType){
    if (busy()) 
        return false;
    else
    {
        EntityData unit = entityDataMap.at(unitType);
        if (rm->canBuild(unit))
        {
            rm->consumeMinerals(unit.minerals);
            rm->consumeVespene(unit.vespene);
            rm->consumeSupply(unit.supplyCost);
            workTimeRemaining = unit.buildTime;
            producing = true;
            logger_->addBuildstart(BuildStartEntry(entityNameMap.at(unitType), id));
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

bool CommandCenter::upgrade(EntityType upgrade){
    if (getType() != command_center) return false;
    if (busy()) return false;
    
    EntityData upgradeEntity = entityDataMap.at(upgrade);
    if (rm->canBuild(upgradeEntity)){
        rm->consumeMinerals(upgradeEntity.minerals);
        rm->consumeVespene(upgradeEntity.vespene);
        entityData = &entityDataMap.at(upgrade);
        
        underConstruction = true;
        constrTimeRemaining = upgradeEntity.buildTime;

        logger_->addBuildstart(BuildStartEntry(getName(), id));
        return true;
    }
    return false;
    
}
