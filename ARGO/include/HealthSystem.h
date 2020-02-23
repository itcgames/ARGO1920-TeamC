#pragma once
#include <iostream>
#include "SDL.h"
#include "Debug.h"
#include "BaseSystem.h"
#include "Entity.h"
#include "HealthComponent.h"

class HealthSystem : public BaseSystem
{
public:
	~HealthSystem();
	void update(Entity& t_entity);
	void update(Entity& t_entity, float t_deltaTime);
};

