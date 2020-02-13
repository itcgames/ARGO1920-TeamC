#pragma once
#include <iostream>
#include <SDL.h>
#include "Controller.h"
#include "MacroCommand.h"
#include <gtc/random.hpp>
#include "Entity.h"
#include "HealthComponent.h"
#include "TransformComponent.h"
#include "InputComponent.h"
#include "ColourComponent.h"
#include "VisualComponent.h"
#include "ColliderAABBComponent.h"
#include "ColliderCircleComponent.h"
#include "HealthSystem.h"
#include "PhysicsSystem.h"
#include "InputSystem.h"
#include "RenderSystem.h"
#include "AiSystem.h"
#include "FiniteStateMachine.h"
#include "EventManager.h"
#include "CollisionSystem.h"

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
	void processEvent();
	void update(bool t_canTick, bool t_canRender, Uint16 t_dt);
	void preRender();
	void cleanup();
	void setupLevel();
	bool checkCanRender(Uint16 t_currentTick);
	bool checkCanTick(Uint16 t_currentTick);
	void closeWindow(const CloseWindow& t_event);

	const int MAX_PLAYERS = 4;
	const int MAX_ENTITIES = 10000;
	const int PLAYER_MAX_HEALTH = 10;
	EventManager m_eventManager;

	HealthSystem m_hpSystem;
	PhysicsSystem m_transformSystem;
	InputSystem m_inputSystem;
	RenderSystem m_renderSystem;
	AiSystem m_aiSystem;
	CollisionSystem m_collisionSystem;

	Entity m_players[4];
	std::vector<Entity> m_entities;
	std::vector<Entity> m_levelTiles;

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