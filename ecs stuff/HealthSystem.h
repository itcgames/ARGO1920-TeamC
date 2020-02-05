#pragma once
#include "Entity.h"
#include "SDL.h"
#include "HealthComponent.h"
#include <iostream>
#include "Debug.h"

class HealthSystem
{
public:
	void addEntity(Entity* t_e);
	void update();
private:
	std::vector<Entity*> m_entities;
	//std::vector<HealthComponent*> m_hpComps;

	Uint32 m_timer = 0;
	const Uint32 UPDATE_DELAY = 500;

};

