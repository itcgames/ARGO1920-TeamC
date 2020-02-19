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
#include "ParticleSystem.h"
#include "LevelManager.h"

class GameScreen
{
public:

	GameScreen(SDL_Renderer* t_renderer, EventManager& t_eventManager, Controller t_controller[Utilities::NUMBER_OF_PLAYERS], ButtonCommandMap t_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS][Utilities::NUMBER_OF_PLAYERS]);
	~GameScreen();

	void update(Uint16 t_deltaTime);
	void processEvents(SDL_Event* t_event);
	void render(SDL_Renderer* t_renderer);
private:

	void createPlayer(Entity& t_player, int t_index);
	void createEnemy();
 	void setUpLevel();

	void preRender();
	void updatePlayers();
	void updateEntities();
	void updateProjectiles();
	void setControllerButtonMap(ButtonCommandMap t_controllerMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS][Utilities::NUMBER_OF_PLAYERS]);




	void removeDeadEnemies();




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
	ParticleSystem m_particleSystem;


	ProjectileManager m_projectileManager;
	LevelManager m_levelManager;


	ButtonCommandMap m_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS][Utilities::NUMBER_OF_PLAYERS]; 
};

