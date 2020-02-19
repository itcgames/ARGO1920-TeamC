#pragma once
#include "EventManager.h"
#include "Controller.h"

class LicenseScreen
{
public:
	LicenseScreen(EventManager& t_eventManager, Controller& t_controller, SDL_Renderer* t_renderer);
	~LicenseScreen();

	void update(Uint16 t_deltaTime);
	void render(SDL_Renderer* t_renderer);
	void reset();
private:
	EventManager& m_eventManager;
	Controller& m_controller;
};

