#pragma once
#include <iostream>
#include <SDL.h>
#include "Controller.h"
#include "InputHandler.h"
#include "MacroCommand.h"
#include <gtc/random.hpp>
#include "Entity.h"
#include "HealthComponent.h"
#include "TransformComponent.h"
#include "InputComponent.h"
#include "ColourComponent.h"
#include "VisualComponent.h"
#include "HealthSystem.h"
#include "PhysicsSystem.h"
#include "InputSystem.h"
#include "RenderSystem.h"
#include "AiSystem.h"

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
	void update();
	void preRender();
	void postRender();
	void render();
	void cleanup();
	void setupLevel();

	const int MAX_PLAYERS = 4;
	const int MAX_ENTITIES = 10000;
	const int PLAYER_MAX_HEALTH = 10;

	HealthSystem m_hpSystem;
	PhysicsSystem m_transformSystem;
	InputSystem m_inputSystem;
	RenderSystem m_renderSystem;
	AiSystem m_aiSystem;

	Entity m_players[4];
	std::vector<Entity> m_entities;
	std::vector<Entity> m_levelTiles;


	// Window used for the game
	SDL_Window* m_window;
	// Renderer used to render onto screen
	SDL_Renderer* m_renderer;

	// bool for if game is running or not
	bool m_isRunning;
 
	//2D grid of tiles
	int m_levelWidth; //TODO: Move to global space
	int m_levelHeight; //TODO: Move to global space
	int m_tileSize;

	Uint16 m_timePerFrame;
	Uint16 m_timePerTick;
	Uint16 m_lastTick;
	Uint16 m_lastRender;
	Uint16 m_fps;
	Uint16 m_tps;
};