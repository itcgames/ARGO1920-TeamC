#pragma once
#include "stdafx.h"
#include <iostream>
#include <SDL.h>
#include "ImguiHelper.h"

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
	ImVec4 clear_color = ImVec4(0, 0, 0, 255);
	float counter = 0;
	std::string windowTitle = "ROCK";
	std::string imguiWindowTitle;
	glm::vec2 vec2 = glm::vec2(0, 0);
	glm::vec3 vec3 = glm::vec3(0, 0, 0);
	
};

