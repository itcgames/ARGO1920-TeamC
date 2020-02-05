#pragma once

#include <memory>
#include <iostream>

#include "SDL.h"
#include "Entity.h"
#include "Debug.h"

class BaseSystem
{
public:
	virtual ~BaseSystem() = 0;
	virtual void addEntity(Entity* t_e);
	virtual void update() = 0;
protected:
	std::vector<std::shared_ptr<Entity>> m_entities;
};

