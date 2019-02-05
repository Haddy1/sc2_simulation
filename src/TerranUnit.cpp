#include "TerranUnit.h"

TerranUnit::TerranUnit(int& ID_Counter, EntityType unitType):
      Unit(ID_Counter, unitType)
{}

TerranUnit::~TerranUnit() {}
SCV::SCV(int& ID_Counter, EntityType unitType, JsonLoggerV2* eventList, bool logging):
    TerranUnit(ID_Counter, unitType)
    ,logger_(eventList)
    ,logging_(logging)

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
                buildings.cCenterList.emplace_back(ID_Counter, buildType, rm, tech, units, logger_, getID(), logging_);
            }

            // check if unit producing building
            else if ( buildings.factoryList.find(buildType) != buildings.factoryList.end()){
                buildings.factoryList.at(buildType).emplace_back(ID_Counter, buildType, rm, tech, units, logger_, getID(), logging_);
            }
            // generic building is left
            else {
                buildings.buildingList.emplace_back(ID_Counter, buildType, rm, tech, units, logger_, getID(), logging_);
            }

            if (logging_)
                logger_->addBuildstart(BuildStartEntry(entityNameMap.at(buildType), id));

            return true;

        }
        else{
            return false;
        }

    }

    return false;

}
