#include "../include/ForwardSimulator.h"
#include "../include/EntityData.h"
#include "../include/TerranBuilding.h"
#include "../include/TerranUnit.h"

#include <iostream>
TerranSimulator::TerranSimulator(queue<EntityType> q, bool log):
    buildOrder(q)
    ,rm()
    ,buildings()
    ,units()
    ,tech()
    ,timestep(1)
    ,running(true)
    ,logger(Race::TERRAN, rm, true)
    ,logging(log)
    ,ID_Counter(0)
    ,maxTime_(1000)
{}

TerranSimulator::TerranSimulator(queue<EntityType> q, bool log, int maxTime):
    buildOrder(q)
    ,rm()
    ,buildings()
    ,units()
    ,tech()
    ,timestep(1)
    ,running(true) 
    ,logger(Race::TERRAN, rm, true)
    ,logging(log)
    ,ID_Counter(0)
    ,maxTime_(maxTime)
{}


void TerranSimulator::init() {
    if (logging)
        logger.printBeginning();

    CommandCenter cCenter(ID_Counter, command_center, &rm, &tech, &units, &logger, logging);
    buildings.cCenterList.push_back(cCenter);
    tech.add(command_center);
    vector<int> initCCenterId = {cCenter.getID()};
    rm.addSupplyMax(entityDataMap.at(command_center).supplyProvided);

    vector<int> initWorkerIds;
    for (int i = 0; i < 6; i++){
        SCV newWorker(ID_Counter, scv, &logger, logging);
        units.workerList.insert(std::pair<int,SCV>(newWorker.getID(), newWorker));
        rm.consumeSupply(newWorker.getEntityData()->supplyCost);
        initWorkerIds.push_back(newWorker.getID());
    }
    rm.setMineralWorkers(6);

    if (logging){
        vector<pair<string, vector<int>>> initUnits;
        initUnits.emplace_back("command_center", initCCenterId);
        initUnits.emplace_back("scv", initWorkerIds);
        logger.printSetup(initUnits);
    }

}

void TerranSimulator::simulate() {

    running = true;
    bool buildingsIdle = true;
    while(running and timestep <= maxTime_){
        rm.update();
        //std::cout << rm.getMinerals() << std::endl;

        buildingsIdle = true;
        // Update all regular buildings
            for (size_t i = 0; i < buildings.buildingList.size(); ++i){
                buildings.buildingList[i].update();
                if (buildings.buildingList[i].busy()) 
                    buildingsIdle = false;
            }
        // Update all producing buildings
        std::unordered_map<EntityType, vector<FactoryBuilding>>::iterator itFactory;
        for ( auto& it : buildings.factoryList){
            for (size_t i = 0; i < it.second.size(); ++i){
                it.second[i].update(ID_Counter);
                if (it.second[i].busy())
                    buildingsIdle = false;
            }
        }

        // Update cCenter construction
        for ( size_t i = 0; i < buildings.cCenterList.size(); ++i){
            buildings.cCenterList[i].update(ID_Counter);
            if (buildings.cCenterList[i].busy())
                buildingsIdle = false;
        }

        if (! buildOrder.empty()){
            bool calledMule = false;
            // Update cCenter construction
            for ( size_t i = 0; i < buildings.cCenterList.size(); ++i){
                if (buildings.cCenterList[i].callMule()){
                    calledMule = true;
                    break;
                }
            }
            if (! calledMule){

                EntityType itemType = buildOrder.front();
                EntityData item = entityDataMap.at(itemType);

                if (tech.dependencyFulfilled(item) && rm.canBuild(item)){
                    EntityType producer = item.producedBy[0];

                    // redirect producer to the base building for tech_lab, so only one list is necessary
                    if (FactoryBuilding::labBaseBuildings.find(producer) != FactoryBuilding::labBaseBuildings.end()){
                        producer = FactoryBuilding::labBaseBuildings.at(producer);
                    }
                    buildingsIdle = false;

                    if (itemType == scv) {
                        for ( size_t i = 0; i < buildings.cCenterList.size(); ++i){
                            if (buildings.cCenterList[i].createUnit(itemType)){
                                buildOrder.pop();
                                break;
                            }
                        }
                    }
                    else if (producer == scv){
                    std::unordered_map<int, SCV>::iterator itSCV;
                        for ( itSCV = units.workerList.begin(); itSCV != units.workerList.end(); ++itSCV){
                            if (itSCV->second.construct(ID_Counter, itemType, &rm, &tech, buildings, &units)){
                                buildOrder.pop();
                                break;
                            }
                        }
                    }
                    else if (buildings.factoryList.find(producer) != buildings.factoryList.end()){
                        if (FactoryBuilding::labBaseBuildings.find(itemType) != FactoryBuilding::labBaseBuildings.end()){
                            for (FactoryBuilding& factoryBuilding : buildings.factoryList.at(producer)){
                                if (factoryBuilding.buildAddon(itemType, FactoryBuilding::tech_lab)){
                                    buildOrder.pop();
                                    break;
                                }
                            }
                        }
                        else if (FactoryBuilding::reactorBaseBuildings.find(itemType) != FactoryBuilding::reactorBaseBuildings.end()){
                            for (FactoryBuilding& factoryBuilding : buildings.factoryList.at(producer)){
                                if (factoryBuilding.buildAddon(itemType, FactoryBuilding::reactor)){
                                    buildOrder.pop();
                                    break;
                                }
                            }
                        }
                        else {
                            for (FactoryBuilding& factoryBuilding : buildings.factoryList.at(producer)){
                                if (factoryBuilding.createUnit(itemType)){
                                    buildOrder.pop();
                                    break;
                                }   
                            }
                        }
                    }
                    else if (itemType == orbital_command || itemType == planetary_fortress){
                        for ( size_t i = 0; i < buildings.cCenterList.size(); ++i){
                            if (buildings.cCenterList[i].upgrade(itemType)){
                                buildOrder.pop();
                                break;
                            }
                        }
                    }
                    else {
                        std::cerr << entityNameMap.at(itemType) << " could not be handled!" << std::endl;
                    }
                }
            }
        }

        int freeWorkers = 0;
        std::unordered_map<int, SCV>::iterator itSCV;
            for ( itSCV = units.workerList.begin(); itSCV != units.workerList.end(); ++itSCV){
            if (! itSCV->second.busy)
                freeWorkers += 1;
        }

        int nrRefineries = rm.getGeysers();
        int vespeneWorkers = std::min(freeWorkers, 3 * nrRefineries);
        rm.setVespeneWorkers(vespeneWorkers);
        freeWorkers -= vespeneWorkers;
        rm.setMineralWorkers(freeWorkers);

        if (logging){
            logger.printMessage(timestep);
        }

        if (buildOrder.empty() and buildingsIdle == true)
            running = false;

        timestep += 1;
    }
    if (timestep > maxTime_){
        timeout = true;
    }
    if (logging)
        logger.printEnd();
}

int TerranSimulator::numberOfUnits(EntityType target){
    if (target == scv)
        return units.workerList.size();
    else if (target == command_center || target == orbital_command || target == planetary_fortress){
        int count = 0;
        for (auto const& entry : buildings.cCenterList){
            if (entry.getType() == target)
                count += 1;
        }
        return count;
    }
    // producer is scv => building
    else if (entityDataMap.at(target).producedBy[0] == scv){
        if (buildings.factoryList.find(target) != buildings.factoryList.end()){
            return buildings.factoryList.at(target).size();
        }
        // Generic buildings left
        else {
            int count = 0;
            for (size_t i = 0; i < buildings.buildingList.size(); ++i)
                if (buildings.buildingList[i].getType() == target)
                    ++count;
            return count;
        }
    }
    // reactors and tech_labs have to be handled by comparing entry for entry
    else if (FactoryBuilding::reactorBaseBuildings.find(target) != FactoryBuilding::reactorBaseBuildings.end()){
        int count = 0;
        std::unordered_map<EntityType, vector<FactoryBuilding>>::iterator itFactory;
        for ( itFactory = buildings.factoryList.begin(); itFactory != buildings.factoryList.end(); ++itFactory){
            for (FactoryBuilding& factory : itFactory->second){
                if (factory.getAddon() == target)
                    count += 1;
            }
        }
        return count;
    }
    else if (FactoryBuilding::labBaseBuildings.find(target) != FactoryBuilding::labBaseBuildings.end()){
        int count = 0;
        std::unordered_map<EntityType, vector<FactoryBuilding>>::iterator itFactory;
        for ( itFactory = buildings.factoryList.begin(); itFactory != buildings.factoryList.end(); ++itFactory){
            for (FactoryBuilding& factory : itFactory->second){
                if (factory.getAddon() == target)
                    count += 1;
            }
        }
        return count;
    }
    // Generic Unit Left
    else {
        return units.unitList.at(target).size();
    }
}

int TerranSimulator::numberOfProductionStructures(){
    int count = 0;
    count += buildings.cCenterList.size();
    for (auto const& entry : buildings.factoryList){
        count += entry.second.size();
    }
    return count;
}
