#pragma once
#include "Entity.h"

//实体飞行器
class EAircraft :public Entity
{
public:
    enum Type{Host, Slave};
public:
    explicit EAircraft(Type type);
private:
    Type mType;
};

