#include "../include/ForwardSimulator.h"
#include "../include/EntityData.h"
#include "../include/TerranBuilding.h"
#include "../include/TerranUnit.h"

#include <iostream>

TerranSimulator::TerranSimulator() :
    rm()
    ,timestep(0)
    ,running(true) 
    ,logger(Race::TERRAN, rm, true)
{}

TerranSimulator::TerranSimulator(queue<string> q) : buildOrder(q)
    ,rm()
    ,timestep(0)
    ,running(true) 
    ,logger(Race::TERRAN, rm, true)
{}


void TerranSimulator::init() {
    rm.setMineralWorkers(6);
    vector<int> initWorkerIds = {0,1,2,3,4,5};
    for (int i = 0; i < 6; i++){
        SCV newWorker("scv", &eventList_);
        SCV::workerList.emplace_back("scv", &eventList_);
        rm.consumeSupply(entityDataMap.at("scv").supplyCost);
    }
    CommandCenter cCenter("command_center", &eventList_, &rm);
    CommandCenter::cCenterList.push_back(cCenter);
    rm.addSupplyMax(entityDataMap.at("command_center").supplyProvided); 
    vector<int> cCenterId = {0};

    vector<pair<string, vector<int>>> initUnits;
    initUnits.emplace_back("drone", initWorkerIds);
    initUnits.emplace_back("command_center", cCenterId);
    logger.printSetup(initUnits);

}

void TerranSimulator::simulate() {
    while(running and timestep < 1000){
        
        // Update all regular buildings
        std::unordered_map<string, vector<std::shared_ptr<TerranBuilding>>>::iterator itBuilding;
        for ( itBuilding = TerranBuilding::buildingList.begin(); itBuilding != TerranBuilding::buildingList.end(); ++itBuilding){
            for (std::shared_ptr<TerranBuilding> building : itBuilding->second)
                building->update();
        }
        // Update all producing buildings
        std::unordered_map<string, vector<std::shared_ptr<FactoryBuilding>>>::iterator itFactory;
        for ( itFactory = FactoryBuilding::factoryList.begin(); itFactory != FactoryBuilding::factoryList.end(); ++itFactory){
            for (std::shared_ptr<FactoryBuilding> factory : itFactory->second)
                factory->update();
        }

        for ( CommandCenter CCenter : CommandCenter::cCenterList){
            CCenter.update();
        }

        for ( CommandCenter CCenter : CommandCenter::cCenterList){
            CCenter.callMule();
        }

        for (MULE mule : MULE::muleList){
            mule.update();
        }

        string buildName = buildOrder.front();

        string producer = entityDataMap.at(buildName).producedBy[0];

        if (! dependencyFulfilled(entityDataMap.at(buildName))){
        }
        else if (rm.canBuild(entityDataMap.at(buildName))){

            if (buildName == "scv") {
                for ( CommandCenter CCenter : CommandCenter::cCenterList){
                    if (CCenter.createUnit(buildName)){
                        buildOrder.pop();
                        break;
                    }
                }
            }
        }
        else if (producer == "scv"){
            for (SCV scv : SCV::workerList){
                if (scv.construct(buildName, &rm)){
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
            for ( CommandCenter CCenter : CommandCenter::cCenterList){
                if (CCenter.upgrade(buildName)){
                    buildOrder.pop();
                    break;
                }
            }
        }
        else {
            running = false;
        }

        int freeWorkers = 0;
        for (SCV scv : SCV::workerList){
            if (! scv.busy){
                freeWorkers += 1;
            }
        }

        int nrRefineries = TerranBuilding::buildingList.at("refinery").size();
        int vespeneWorkers = freeWorkers - 3 * nrRefineries;
        if (vespeneWorkers < 0) vespeneWorkers = freeWorkers;
        rm.setVespeneWorkers(vespeneWorkers);
        freeWorkers -= vespeneWorkers;
        int nrMules = MULE::muleList.size();
        rm.setMineralWorkers(4 * nrMules + freeWorkers);
        
        vector<EventEntry*> eventPointers;
        for (EventEntry event : eventList_){
            eventPointers.push_back(&event);
        }

        logger.printMessage(timestep, eventPointers);
        eventPointers.clear();
        eventList_.clear();

        timestep += 1;
    }

}
