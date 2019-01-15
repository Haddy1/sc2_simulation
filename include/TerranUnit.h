#pragma once 

#include <vector>
#include <string>
#include <memory>
#include <assert.h>
#include <algorithm>
#include "TerranBuilding.h"
#include "ResourceManager.h"
#include "Tech.h"
#include "Unit.h"
#include "JsonLoggerV2.h"

using std::vector;
using std::string;

class TerranUnit : public Unit
{
    bool constructing = false;
protected:
    string name_;

public:
    static vector<TerranUnit> unitList ;
    TerranUnit(int& ID_Counter, string name);
    ~TerranUnit();
};

class SCV : public TerranUnit
{
    string name_;
    JsonLoggerV2* logger_;
	Tech& tech;
public:
    static vector<SCV> workerList;
    SCV(int& ID_Counter, string name, JsonLoggerV2* eventList, Tech& tech);
    ~SCV();
    bool construct(int& ID_Counter, string building, ResourceManager* rm);
    bool busy = false;
};

class MULE 
{
    public:
        static vector<MULE> muleList;
    private:
        ResourceManager* rm;
        JsonLoggerV2* logger_;
        int initLifeTime = 90;
        int lifeTime = 90;

public:
    MULE(ResourceManager* resourceManager, JsonLoggerV2* eventList);
    ~MULE();
    void update();
    const static int energyCost = 50;
};

