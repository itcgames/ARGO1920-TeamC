#pragma once
#include "stdafx.h"
#include <iostream>
#include <SDL.h>
#include "Tile.h"
#include "FiniteStateMachine.h"

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
	void render();
	void cleanup();

	void setupLevel();

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
	std::vector<Tile> m_levelTiles;

	FiniteStateMachine fsm;
};

