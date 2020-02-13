#pragma once
#include "Entity.h"
#include "EventManager.h"

class BaseSystem
{
public:
 	virtual ~BaseSystem();
	virtual void update(Entity& t_entity) = 0;
};

