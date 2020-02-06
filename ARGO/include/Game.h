#pragma once
#include "stdafx.h"
#include <iostream>
#include <SDL.h>

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

	// Window used for the game
	SDL_Window* m_window;
	// Renderer used to render onto screen
	SDL_Renderer* m_renderer;
	// bool for if game is running or not
	bool m_isRunning;

	bool showDemoWindow = true;
	bool showanotherWindow = true;
	bool done = false;
	ImVec4 clear_color = ImVec4(0, 0, 0, 0);
	int counter = 0;

};

