#pragma once
#include "Debug.h"
#include <iostream>
#include "SDL.h"
#include "PositionComponent.h"
#include "Entity.h"

class RenderSystem
{
public:
	void addEntity(Entity* t_e);
	void render(SDL_Renderer* t_renderer);
private:
	std::vector<Entity*> m_entities;
	std::vector<PositionComponent*> m_posComps;
	std::vector<int> m_rgbVec;
};

