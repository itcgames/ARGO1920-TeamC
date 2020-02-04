#pragma once
#include "InputComponent.h"
#include "SDL.h"
#include "Entity.h"
#include <iostream>
#include "Debug.h"

class InputSystem
{
public:
	void addEntity(Entity* t_e);
	void update();
private:
	std::vector<Entity*> m_entities;
	std::vector<InputComponent*> m_inputComps;
};

