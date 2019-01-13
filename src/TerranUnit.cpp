#include "TerranUnit.h"

vector<TerranUnit> TerranUnit::unitList = vector<TerranUnit>();
vector<SCV> SCV::workerList = vector<SCV>();

TerranUnit::TerranUnit(string name):
      Unit(name)
      ,name_(name) 
{}

TerranUnit::~TerranUnit() {}

SCV::SCV(string name, vector<EventEntry>* eventList):
    TerranUnit(name)
    ,eventList_(eventList)

{}

SCV::~SCV() {}

bool SCV::construct(string buildingName, ResourceManager* rm){
    if (busy) return false;
    else {
        EntityData building = entityDataMap.at(buildingName);
        if (rm->canBuild(building)){
            busy = true;
            rm->consumeMinerals(building.minerals);
            rm->consumeVespene(building.vespene);

            
            // check if command center
            if ( buildingName == "command_center") {
                CommandCenter::cCenterList.emplace_back(buildingName, rm, eventList_, this);
            }

            // check if unit producing building
            else if ( FactoryBuilding::factoryList.find(buildingName) != FactoryBuilding::factoryList.end()){
                std::shared_ptr<FactoryBuilding> newBuilding(new FactoryBuilding(buildingName, rm, eventList_, this));
                FactoryBuilding::factoryList.at(buildingName).push_back(newBuilding);
            }
            // generic building is left
            else {
                std::shared_ptr<TerranBuilding> newBuilding(new TerranBuilding(buildingName, rm, eventList_, this));
                TerranBuilding::buildingList.at(buildingName).push_back(newBuilding);
            }

            eventList_->emplace_back("build-start", buildingName, id);

            return true;

        }
        else{
            return false;
        }

    }

    return false;

}


MULE::MULE(ResourceManager* resourceManager, vector<EventEntry>* eventList):
    rm(resourceManager)
    , eventList_(eventList)
{
    rm->setMineralWorkers(rm->getMineralWorkers() + 4);
}
MULE::~MULE(){
    assert (rm->getMineralWorkers() >= 4 && "MULE removed with less than 4 minerals workers remaining");
    rm->setMineralWorkers(rm->getMineralWorkers() - 4);
}

void MULE::update(){
    if (lifeTime > 0)
        --lifeTime;
    else {
        MULE::muleList.pop_back();
        delete this;
    }
}
