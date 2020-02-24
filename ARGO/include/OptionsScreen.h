#pragma once
#include "EventManager.h"
#include "Controller.h"

class OptionsScreen
{
public:
	OptionsScreen(EventManager& t_eventManager, Controller& t_controller, SDL_Renderer* t_renderer);
	~OptionsScreen();

	void update(float t_deltaTime);
	void render(SDL_Renderer* t_renderer);
	void reset();
	void initialise();
	void passSystems();
private:
	EventManager& m_eventManager;
	Controller& m_controller;
};

