#pragma once
#include "EntityFactory.h"
class PickUpFactory :
	public EntityFactory
{
public:
	PickUpFactory(SDL_Renderer* t_renderer);
	void createPlayer(Entity& t_entity);
	void createEnemy(int choice, Entity& t_entity);
	void createDrop(int choice, Entity& t_entity);
private:
	SDL_Renderer* m_renderer;
};


