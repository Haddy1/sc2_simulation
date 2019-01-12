#pragma once 

#include <vector>
#include <string>
#include <memory>
#include <assert.h>
#include <algorithm>
#include "TerranBuilding.h"
#include "ResourceManager.h"
#include "Tech.h"
#include "JsonLogger.h"

using std::vector;
using std::string;

class TerranUnit //: public Unit
{
    bool constructing = false;
protected:
    string id;
    string name_;

public:
    static vector<TerranUnit> unitList ;
    TerranUnit(string name);
    ~TerranUnit();
    string getId(){return id;}

};

class SCV : public TerranUnit
{
    string name_;
    vector<EventEntry>* eventList_;
public:
    static vector<SCV> workerList;
    SCV(string name, vector<EventEntry>* eventList);
    ~SCV();
    bool construct(string building, ResourceManager* resourceManager);
    bool busy = false;
    string getId(){return id;}
};

class MULE 
{
    public:
        static vector<MULE> muleList;
    private:
        ResourceManager* rm;
        vector<EventEntry>* eventList_;
        int lifeTime = 90;

public:
    MULE(ResourceManager* resourceManager, vector<EventEntry>* eventList);
    ~MULE();
    void update();
    const static int energyCost = 50;
};

