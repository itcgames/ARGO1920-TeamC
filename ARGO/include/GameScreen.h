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
#include "OnlineGameHandler.h"
#include <sstream>

class GameScreen
{
public:

	GameScreen(SDL_Renderer* t_renderer, EventManager& t_eventManager, Controller t_controller[Utilities::S_MAX_PLAYERS], CommandSystem& t_commandSystem, InputSystem& t_input, RenderSystem& t_renderSystem, OnlineGameHandler& t_onlineHandler);
	~GameScreen();

	void update(float t_deltaTime);
	void processEvents(SDL_Event* t_event);
	void render(SDL_Renderer* t_renderer);
	void reset(SDL_Renderer* t_renderer, Controller t_controller[Utilities::S_MAX_PLAYERS]);
	void initialise(SDL_Renderer* t_renderer, ButtonCommandMap t_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS][Utilities::S_MAX_PLAYERS], Controller t_controller[Utilities::S_MAX_PLAYERS], bool t_isOnline = false);
private:

	void createPlayer(Entity& t_player, int t_index, SDL_Renderer* t_renderer, bool t_isOnline);
	void createEnemy();
	void createGoal();
	void setUpLevel();
	void preRender();
	void updatePlayers(float t_deltaTime);
	void updateEntities(float t_deltaTime);
	void updateProjectiles(float t_deltaTime);
	void updateLevelManager();
	void setControllerButtonMap(ButtonCommandMap t_controllerMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS][Utilities::S_MAX_PLAYERS]);
	void removeDeadEnemies();
	void processReceivedClientData();
	void processReceivedHostData();
	void sendHostData();
	void sendClientData();

	EventManager& m_eventManager;

	static const int MAX_ENTITIES = 10000;

	Controller m_controllers[Utilities::S_MAX_PLAYERS];
	Entity m_players[Utilities::S_MAX_PLAYERS];
	std::vector<Entity> m_entities;
	Entity m_goal;
	SDL_Renderer* m_renderer;

	// Systems
	HealthSystem m_healthSystem;
	PhysicsSystem m_transformSystem;
	AiSystem m_aiSystem;
	CollisionSystem m_collisionSystem;
	ParticleSystem m_particleSystem;

	CommandSystem& m_commandSystem;
	InputSystem& m_inputSystem;
	RenderSystem& m_renderSystem;


	ProjectileManager m_projectileManager;
	LevelManager m_levelManager;

	ButtonCommandMap m_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS][Utilities::S_MAX_PLAYERS];

	// bool to see if game is being played online or not
	//bool m_isOnline;

	OnlineGameHandler& m_onlineHandler;
};

