#pragma once
#include "stdafx.h"
#include <iostream>
#include <SDL.h>
#include "Controller.h"
#include "InputHandler.h"
#include "MacroCommand.h"

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
	void initInputHandler();
	// Window used for the game
	SDL_Window* m_window;
	// Renderer used to render onto screen
	SDL_Renderer* m_renderer;
	// bool for if game is running or not
	bool m_isRunning;

	std::vector<Controller*> controllers;

	InputHandler m_inputHandler;
	std::map<ButtonType, Command*> buttonPressedMap;
};

