#pragma once
#include "EventManager.h"
#include "Controller.h"

class CreditsScreen
{
public:
	CreditsScreen(EventManager& t_eventManager, Controller& t_controller, SDL_Renderer* t_renderer);
	~CreditsScreen();

	void update(Uint16 t_deltaTime);
	void render(SDL_Renderer* t_renderer);
	void reset();
private:
	EventManager& m_eventManager;
	Controller& m_controller;
};

