#pragma once
#include "EventManager.h"
#include "Controller.h"

class AchievementScreen
{
public:
	AchievementScreen(EventManager& t_eventManager, Controller& t_controller, SDL_Renderer* t_renderer);
	~AchievementScreen();

	void update(float t_deltaTime);
	void render(SDL_Renderer* t_renderer);
	void reset();
	void initialise();
private:
	EventManager& m_eventManager;
	Controller& m_controller;
};