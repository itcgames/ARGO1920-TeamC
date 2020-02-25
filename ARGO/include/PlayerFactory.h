#pragma once
#include "EntityFactory.h"
class PlayerFactory :
	public EntityFactory
{
public:
	PlayerFactory();
	void initialise(SDL_Renderer* t_renderer);
	void createPlayer(Entity& t_entity);
	void createPlayer(Entity& t_entity, bool t_isPlayer, Controller& t_controllers, int t_index, ButtonCommandMap t_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS][Utilities::S_MAX_PLAYERS]);
	void createEnemy(int t_type, Entity& t_entity);
	void createDrop(int t_type, Entity& t_entity);

private:
	SDL_Renderer* m_renderer;
};

