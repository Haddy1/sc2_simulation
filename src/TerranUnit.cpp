#include "TerranUnit.h"

TerranUnit::TerranUnit(int& ID_Counter, EntityType unitType):
      Unit(ID_Counter, unitType)
{}

TerranUnit::~TerranUnit() {}
SCV::SCV(int& ID_Counter, EntityType unitType, JsonLoggerV2* eventList):
    TerranUnit(ID_Counter, unitType)
    ,logger_(eventList)

{}

SCV::~SCV() {}

bool SCV::construct(int& ID_Counter, EntityType buildType, ResourceManager* rm, Tech* tech, TerranBuildings& buildings, TerranUnits* units){
    if (busy) return false;
    else {
        EntityData building = entityDataMap.at(buildType);
        if (rm->canBuild(building)){
            busy = true;
            rm->consumeMinerals(building.minerals);
            rm->consumeVespene(building.vespene);

            
            // check if command center
            if ( buildType == command_center) {
                buildings.cCenterList.emplace_back(ID_Counter, buildType, rm, tech, units, logger_, this);
            }

            // check if unit producing building
            else if ( buildings.factoryList.find(buildType) != buildings.factoryList.end()){
                //std::shared_ptr<FactoryBuilding> newBuilding(new FactoryBuilding(ID_Counter, buildType, rm, tech, unitList, logger_, this));
                std::shared_ptr<FactoryBuilding> newBuilding(new FactoryBuilding(ID_Counter, buildType, rm, tech, units, logger_, this));
                buildings.factoryList.at(buildType).push_back(newBuilding);
            }
            // generic building is left
            else {
                std::shared_ptr<TerranBuilding> newBuilding(new TerranBuilding(ID_Counter, buildType, rm, tech, logger_, this));
                buildings.buildingList.at(buildType).push_back(newBuilding);
            }

            logger_->addBuildstart(BuildStartEntry(entityNameMap.at(buildType), id));

            return true;

        }
        else{
            return false;
        }

    }

    return false;

}
