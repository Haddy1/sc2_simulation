#include "TerranUnit.h"

vector<TerranUnit> TerranUnit::unitList = vector<TerranUnit>();
vector<SCV> SCV::workerList = vector<SCV>();

TerranUnit::TerranUnit(int& ID_Counter, string name):
      Unit(ID_Counter, name)
      ,name_(name) 
{}

TerranUnit::~TerranUnit() {}

SCV::SCV(int& ID_Counter, string name, JsonLoggerV2* eventList, Tech& tech):
    TerranUnit(ID_Counter, name)
    ,logger_(eventList), tech(tech)

{}

SCV::~SCV() {}

bool SCV::construct(int& ID_Counter, string buildingName, ResourceManager* rm){
    if (busy) return false;
    else {
        EntityData building = entityDataMap.at(buildingName);
        if (rm->canBuild(building)){
            busy = true;
            rm->consumeMinerals(building.minerals);
            rm->consumeVespene(building.vespene);

            
            // check if command center
            if ( buildingName == "command_center") {
                CommandCenter::cCenterList.emplace_back(ID_Counter, buildingName, rm, tech, logger_, this);
            }

            // check if unit producing building
            else if ( FactoryBuilding::factoryList.find(buildingName) != FactoryBuilding::factoryList.end()){
                std::shared_ptr<FactoryBuilding> newBuilding(new FactoryBuilding(ID_Counter, buildingName, rm, tech, logger_, this));
                FactoryBuilding::factoryList.at(buildingName).push_back(newBuilding);
            }
            // generic building is left
            else {
                std::shared_ptr<TerranBuilding> newBuilding(new TerranBuilding(ID_Counter, buildingName, rm, tech, logger_, this));
                TerranBuilding::buildingList.at(buildingName).push_back(newBuilding);
            }

            logger_->addBuildstart(BuildStartEntry(buildingName, id));

            return true;

        }
        else{
            return false;
        }

    }

    return false;

}


MULE::MULE(ResourceManager* resourceManager, JsonLoggerV2* eventList):
    rm(resourceManager)
    , logger_(eventList)
{}
MULE::~MULE(){
}

void MULE::update(){
    if (lifeTime >= 0){
        if (lifeTime < initLifeTime) {
            rm->addMinerals(rm->mineralsPerWorkerSecond);
            rm->addMinerals(rm->mineralsPerWorkerSecond);
            rm->addMinerals(rm->mineralsPerWorkerSecond);
            rm->addMinerals(rm->mineralsPerWorkerSecond);
        }
        --lifeTime;
    }
    else {
        MULE::muleList.pop_back();
        delete this;
    }
}
