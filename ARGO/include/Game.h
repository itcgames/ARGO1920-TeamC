#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
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
#include "TextComponent.h"
#include "HealthSystem.h"
#include "PhysicsSystem.h"
#include "InputSystem.h"
#include "RenderSystem.h"
#include "AiSystem.h"
#include "ResourceMng.h"
#include "AssetManager.h"

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
	bool checkCanRender(Uint16 t_currentTick);
	bool checkCanTick(Uint16 t_currentTick);

	AssetManager* m_assetMgr;

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

	Entity m_textTest1;
	Entity m_textTest2;

	TTF_Font* m_font;

	// Window used for the game
	SDL_Window* m_window;
	// Renderer used to render onto screen
	SDL_Renderer* m_renderer;

	// bool for if game is running or not
	bool m_isRunning;
 
	//2D grid of tiles
	int m_levelWidth;
	int m_levelHeight;
	int m_tileSize;

	Uint16 m_timePerFrame;
	Uint16 m_timePerTick;
	Uint16 m_lastTick;
	Uint16 m_lastRender;
	Uint16 m_framesPerSecond;
	Uint16 m_ticksPerSecond;
};