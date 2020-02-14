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

	GameScreen(SDL_Renderer* t_renderer, MenuStates* t_currentScreen, EventManager& t_eventManager);
	~GameScreen();

	void update(bool t_canTick, bool t_canRender, Uint16 t_deltaTime);
	void processEvents(SDL_Event* t_event);
private:

	void createPlayer(Entity& t_player);
	void createEnemy();
	void createButtonMaps();
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

	static const int MAX_PLAYERS = 4;
	static const int MAX_ENTITIES = 10000;

	Entity m_players[MAX_PLAYERS];
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

	// Button Maps
	std::map<ButtonType, Command*> m_buttonPressMap;
	std::map<ButtonType, Command*> m_buttonHeldMap;
	std::map<ButtonType, Command*> m_buttonReleasedMap;
};

