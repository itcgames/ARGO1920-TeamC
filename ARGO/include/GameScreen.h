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
#include "EnemyManager.h"
#include "ParticleSystem.h"
#include "LevelManager.h"

class GameScreen
{
public:

	GameScreen(SDL_Renderer* t_renderer, EventManager& t_eventManager, Controller t_controller[Utilities::S_MAX_PLAYERS]);
	~GameScreen();

	void update(float t_deltaTime);
	void processEvents(SDL_Event* t_event);
	void render(SDL_Renderer* t_renderer);
	void reset(SDL_Renderer* t_renderer, Controller t_controller[Utilities::S_MAX_PLAYERS]);
	void initialise(SDL_Renderer* t_renderer, ButtonCommandMap t_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS][Utilities::S_MAX_PLAYERS], Controller t_controller[Utilities::S_MAX_PLAYERS]);
private:

	void createPlayer(Entity& t_player, int t_index, SDL_Renderer* t_renderer);
	void createGoal();
 	void setUpLevel();
	void preRender();
	void updatePlayers(float t_deltaTime);
	void updateEntities(float t_deltaTime);
	void updateProjectiles(float t_deltaTime);
	void updateLevelManager();
	void setControllerButtonMap(ButtonCommandMap t_controllerMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS][Utilities::S_MAX_PLAYERS]);

	EventManager& m_eventManager;

	static const int MAX_ENTITIES = 10000;

	Controller m_controllers[Utilities::S_MAX_PLAYERS];
	Entity m_players[Utilities::S_MAX_PLAYERS];
	//std::vector<Entity> m_entities;
	Entity m_goal;
	SDL_Renderer* m_renderer;

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
	EnemyManager m_enemyManager;

	ButtonCommandMap m_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS][Utilities::S_MAX_PLAYERS];
};

