#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Controller.h"
#include "MacroCommand.h"
#include <gtc/random.hpp>
#include "Entity.h"
#include "HealthComponent.h"
#include "TransformComponent.h"
#include "InputComponent.h"
#include "ColourComponent.h"
#include "VisualComponent.h"
#include "TextComponent.h"
#include "ColliderAABBComponent.h"
#include "ColliderCircleComponent.h"
#include "TagComponent.h"
#include "HealthSystem.h"
#include "PhysicsSystem.h"
#include "InputSystem.h"
#include "RenderSystem.h"
#include "AiSystem.h"
#include "AssetManager.h"
#include "FiniteStateMachine.h"
#include "ParticleSystem.h"
#include "EventManager.h"
#include "CollisionSystem.h"
#include "ProjectileManager.h"
#include "AudioManager.h"
#include "CommandSystem.h"

/// <summary>
/// Game class needed for the game
/// </summary>
class Game
{
public:
	Game();
	~Game();
	void run();
private:
	void initLibraries();
	void processEvent();
	void update(bool t_canTick, bool t_canRender, Uint16 t_dt);
	void preRender();
	void cleanup();
	void setupLevel();
	void createPlayer(Entity& t_player);
	void createEnemy();
	void setToWall(Entity& t_entity, glm::vec2 t_position);
	void setToFloor(Entity& t_entity, glm::vec2 t_position);
	void playerFireSound(const createBulletEvent& t_event);

	bool checkCanRender(Uint16 t_currentTick);
	bool checkCanTick(Uint16 t_currentTick);
	void closeWindow(const CloseWindow& t_event);
	void setupIgnoredEvents();
	void createRenderer();

	AssetManager* m_assetMgr;
	AudioManager* m_audioMgr;

	const int MAX_PLAYERS = 4;
	const int MAX_ENTITIES = 10000;
	const int PLAYER_MAX_HEALTH = 10;
	EventManager m_eventManager;

	HealthSystem m_hpSystem;
	PhysicsSystem m_transformSystem;
	InputSystem m_inputSystem;
	RenderSystem m_renderSystem;
	AiSystem m_aiSystem;
	ParticleSystem m_particleSystem;
	CollisionSystem m_collisionSystem;
	CommandSystem m_commandSystem;


	Entity m_players[4];
	std::vector<Entity> m_entities;
	std::vector<Entity> m_levelTiles;

	Entity m_textTest1;
	Entity m_textTest2;

	ProjectileManager m_projectileManager;

	// Window used for the game
	SDL_Window* m_window;
	// Renderer used to render onto screen
	SDL_Renderer* m_renderer;

	// bool for if game is running or not
	bool m_isRunning;

	Uint16 m_timePerFrame;
	Uint16 m_timePerTick;
	Uint16 m_lastTick;
	Uint16 m_lastRender;
	Uint16 m_framesPerSecond;
	Uint16 m_ticksPerSecond;
};