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
    ,ID_Counter(0)
{}

TerranSimulator::TerranSimulator(queue<string> q) : 
    buildOrder(q)
    ,rm()
    ,timestep(1)
    ,running(true) 
    ,logger(Race::TERRAN, rm, true)
    ,ID_Counter(0)
{}


void TerranSimulator::init() {
    logger.printBeginning();
    CommandCenter cCenter(ID_Counter, "command_center", &rm, tech, &logger);
    CommandCenter::cCenterList.push_back(cCenter);
    vector<int> initCCenterId = {cCenter.getID()};
    rm.addSupplyMax(entityDataMap.at("command_center").supplyProvided);

    vector<int> initWorkerIds;
    for (int i = 0; i < 6; i++){
        SCV newWorker(ID_Counter, "scv", &logger, tech);
        SCV::workerList.push_back(newWorker);
        rm.consumeSupply(newWorker.getEntityData()->supplyCost);
        initWorkerIds.push_back(newWorker.getID());
    }
    rm.setMineralWorkers(6);

    vector<pair<string, vector<int>>> initUnits;
    initUnits.emplace_back("command_center", initCCenterId);
    initUnits.emplace_back("scv", initWorkerIds);
    logger.printSetup(initUnits);

}

void TerranSimulator::simulate() {
    running = true;
    bool buildingsIdle = true;
    while(running and timestep < maxTime){
        rm.update();
//std::cout << rm.getMinerals() << std::endl;

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
                factory->update(ID_Counter);
                if (factory->busy()) 
                    buildingsIdle = false;
            }
        }

        // Update cCenter construction
        for ( size_t i = 0; i < CommandCenter::cCenterList.size(); ++i){
            CommandCenter::cCenterList[i].update(ID_Counter);
            if (CommandCenter::cCenterList[i].busy())
                buildingsIdle = false;
        }
        /*
        for ( size_t i = 0; i < CommandCenter::cCenterList.size(); ++i){
            CommandCenter::cCenterList[i].callMule();
        }
        */

        for ( size_t i = 0; i < MULE::muleList.size(); ++i){
            MULE::muleList[i].update();
        }


        if (! buildOrder.empty()){
            string buildName = buildOrder.front();
            EntityData item = entityDataMap.at(buildName);

            string producer = item.producedBy[0];
            size_t pos = producer.find("with_tech_lab");
            if (pos != string::npos){
                producer.erase(pos, producer.length());
            }

            if (tech.dependencyFulfilled(item) && rm.canBuild(item)){
                buildingsIdle = false;

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
                        if (SCV::workerList[i].construct(ID_Counter, buildName, &rm)){
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
                    for ( size_t i = 0; i < CommandCenter::cCenterList.size(); ++i){
                        if (CommandCenter::cCenterList[i].upgrade(buildName)){
                            buildOrder.pop();
                            break;
                        }
                    }
                }
                else {
                    std::cerr << buildName << " could not be handled!" << std::endl;
                }
            }
        }

        int freeWorkers = 0;
        for (size_t i = 0; i < SCV::workerList.size(); ++i){
            if (! SCV::workerList[i].busy)
                freeWorkers += 1;
        }
        
        int nrRefineries = 0;
        for (size_t i = 0; i < TerranBuilding::buildingList.at("refinery").size(); i++){
            if (! TerranBuilding::buildingList.at("refinery")[i]->isUnderConstruction())
                nrRefineries += 1;
        }
        int vespeneWorkers = std::min(freeWorkers, 3 * nrRefineries);
        rm.setVespeneWorkers(vespeneWorkers);
        freeWorkers -= vespeneWorkers;
        rm.setMineralWorkers(freeWorkers);


        logger.printMessage(timestep);

        if (buildOrder.empty() and buildingsIdle == true)
            running = false;

        timestep += 1;
    }
    logger.printEnd();

}
