#include "../include/ForwardSimulator.h"
#include "../include/EntityData.h"
#include "../include/TerranBuilding.h"
#include "../include/TerranUnit.h"

#include <iostream>

TerranSimulator::TerranSimulator() :
    rm()
    ,timestep(1)
    ,running(true) 
    ,logger(Race::TERRAN, rm, true)
{}

TerranSimulator::TerranSimulator(queue<string> q) : buildOrder(q)
    ,rm()
    ,timestep(1)
    ,running(true) 
    ,logger(Race::TERRAN, rm, true)
{}


void TerranSimulator::init() {
    CommandCenter cCenter("command_center", &rm, &eventList_);
    CommandCenter::cCenterList.push_back(cCenter);
    rm.addSupplyMax(entityDataMap.at("command_center").supplyProvided);

    vector<int> initWorkerIds;
    for (int i = 0; i < 6; i++){
        SCV newWorker("scv", &eventList_);
        SCV::workerList.push_back(newWorker);
        rm.consumeSupply(newWorker.getEntityData()->supplyCost);
        initWorkerIds.push_back(newWorker.getID());
    }
    rm.setMineralWorkers(6);

    vector<pair<string, vector<int>>> initUnits;
    initUnits.emplace_back("command_center", cCenter.getID());
    initUnits.emplace_back("scv", initWorkerIds);
    logger.printSetup(initUnits);

}

void TerranSimulator::simulate() {
    running = true;
    bool buildingsIdle = true;
    while(running and timestep < maxTime){
        rm.update();
        
        buildingsIdle = true;
        // Update all regular buildings
        std::unordered_map<string, vector<std::shared_ptr<TerranBuilding>>>::iterator itBuilding;
        for ( itBuilding = TerranBuilding::buildingList.begin(); itBuilding != TerranBuilding::buildingList.end(); ++itBuilding){
            for (std::shared_ptr<TerranBuilding> building : itBuilding->second){
                building->update();
                if (building->busy()) 
                    buildingsIdle = false;
            }
        }
        // Update all producing buildings
        std::unordered_map<string, vector<std::shared_ptr<FactoryBuilding>>>::iterator itFactory;
        for ( itFactory = FactoryBuilding::factoryList.begin(); itFactory != FactoryBuilding::factoryList.end(); ++itFactory){
            for (std::shared_ptr<FactoryBuilding> factory : itFactory->second){
                factory->update();
                if (factory->busy()) 
                    buildingsIdle = false;
            }
        }

        // Update cCenter construction
        for ( size_t i = 0; i < CommandCenter::cCenterList.size(); ++i){
            CommandCenter::cCenterList[i].update();
            if (CommandCenter::cCenterList[i].busy())
                buildingsIdle = false;
        }

        for ( size_t i = 0; i < CommandCenter::cCenterList.size(); ++i){
            CommandCenter::cCenterList[i].callMule();
        }

        for (MULE mule : MULE::muleList){
            mule.update();
        }


        if (! buildOrder.empty()){
        string buildName = buildOrder.front();

        string producer = entityDataMap.at(buildName).producedBy[0];

        if (buildName == "scv") {
            for ( size_t i = 0; i < CommandCenter::cCenterList.size(); ++i){
                if (CommandCenter::cCenterList[i].createUnit(buildName)){
                    buildOrder.pop();
                    break;
                }
            }
        }
        else if (producer == "scv"){
            for ( size_t i = 0; i < SCV::workerList.size(); ++i){
                if (SCV::workerList[i].construct(buildName, &rm)){
                    buildOrder.pop();
                    break;
                }
            }
        }
        else if (FactoryBuilding::factoryList.find(producer) != FactoryBuilding::factoryList.end()){
            if (buildName.find("with_tech_lab") != string::npos || buildName.find("with_reactor") != string::npos){
                for (std::shared_ptr<FactoryBuilding> factoryBuilding : FactoryBuilding::factoryList.at(producer)){
                    if (factoryBuilding->buildAddon(buildName)){
                        buildOrder.pop();
                        break;
                    }
                }
            }
            else {
                for (shared_ptr<FactoryBuilding> factoryBuilding : FactoryBuilding::factoryList.at(producer)){
                    if (factoryBuilding->createUnit(buildName)){
                        buildOrder.pop();
                        break;
                    }   
                }
            }
        }
        else if (buildName == "orbital_command" || buildName == "planetary_fortress"){
            for ( CommandCenter cCenter : CommandCenter::cCenterList){
                if (cCenter.upgrade(buildName)){
                    buildOrder.pop();
                    break;
                }
            }
        }
        else {
            std::cerr << buildName << " could not be handled!" << std::endl;
        }
        }

        int freeWorkers = 0;
        for (size_t i = 0; i < SCV::workerList.size(); ++i){
            if (! SCV::workerList[i].busy)
                freeWorkers += 1;
        }

        int nrRefineries = TerranBuilding::buildingList.at("refinery").size();
        int vespeneWorkers = std::min(freeWorkers, 3 * nrRefineries);
        rm.setVespeneWorkers(vespeneWorkers);
        freeWorkers -= vespeneWorkers;
        int nrMules = MULE::muleList.size();
        rm.setMineralWorkers(4 * nrMules + freeWorkers);
        
        vector<EventEntry*> eventPointers;
        for (size_t i = 0; i < eventList_.size(); ++i){
            eventPointers.push_back(&eventList_[i]);
        }

        logger.printMessage(timestep, eventPointers);
        eventPointers.clear();
        eventList_.clear();

        if (buildOrder.empty() and buildingsIdle == true)
            running = false;

        timestep += 1;
    }

}
