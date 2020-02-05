#pragma once
#include "Entity.h"
#include "SDL.h"
#include "PositionComponent.h"
#include "InputComponent.h"
#include <iostream>
#include "Debug.h"

class PositionSystem
{
public:
	void addEntity(Entity* t_e);
	void update();
private:
	std::vector<Entity*> m_entities;
	//std::vector<PositionComponent*> m_posComps;
	//std::vector<int> m_entitiesWithInput;
	//std::vector<int> m_inputCompIndex;

	void checkBorder(PositionComponent* t_pos);
	void updateWithInput(PositionComponent* t_pos, InputComponent* t_input);

	Uint32 m_timer;
	const Uint32 UPDATE_DELAY = 1000;
};
