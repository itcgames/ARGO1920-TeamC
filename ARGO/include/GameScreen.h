#pragma once
#include "Utilities.h"
#include "EventManager.h"
#include "HealthSystem.h"
#include "PhysicsSystem.h"
#include "InputSystem.h "
#include "RenderSystem.h"
#include "AiSystem.h"
#include "CollisionSystem.h"
#include "CommandSystem.h"
#include "ProjectileManager.h"

class GameScreen
{
public:

	GameScreen(SDL_Renderer* t_renderer, MenuStates* t_currentScreen, EventManager& t_eventManager, Controller t_controller[Utilities::NUMBER_OF_PLAYERS], ButtonCommandMap t_controllerButtonMaps[][] );
	~GameScreen();

	void update(bool t_canTick, bool t_canRender, Uint16 t_deltaTime);
	void processEvents(SDL_Event* t_event);
private:

	void createPlayer(Entity& t_player, int t_index);
	void createEnemy();
 	void setUpLevel();
	void setToFloor(Entity& t_entity, glm::vec2 t_position);
	void setToWall(Entity& t_entity, glm::vec2 t_position);

	void preRender();
	void updatePlayers(bool t_canTick, bool t_canRender);
	void updateEntities(bool t_canTick, bool t_canRender);
	void updateLevelTiles(bool t_canTick, bool t_canRender);
	void updateProjectiles(bool t_canTick, bool t_canRender);

	SDL_Renderer* m_renderer;
	MenuStates* m_currentScreen;
	EventManager& m_eventManager;

 	static const int MAX_ENTITIES = 10000;

	Controller m_controllers[Utilities::NUMBER_OF_PLAYERS];
	Entity m_players[Utilities::NUMBER_OF_PLAYERS];
	std::vector<Entity> m_entities;
	std::vector<Entity> m_levelTiles;

	// Systems
	HealthSystem m_healthSystem;
	PhysicsSystem m_transformSystem;
	InputSystem m_inputSystem;
	RenderSystem m_renderSystem;
	AiSystem m_aiSystem;
	CollisionSystem m_collisionSystem;
	CommandSystem m_commandSystem;

	ProjectileManager m_projectileManager;


	ButtonCommandMap m_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS][Utilities::NUMBER_OF_PLAYERS]; 
};

