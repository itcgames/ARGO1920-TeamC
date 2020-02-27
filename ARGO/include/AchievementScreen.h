#pragma once
#include "EventManager.h"
#include "Controller.h"
#include "InputSystem.h"
#include "CommandSystem.h"
#include "RenderSystem.h"

class AchievementScreen
{
public:
	AchievementScreen(EventManager& t_eventManager, CommandSystem& t_commandSystem, InputSystem& t_inputSystem, RenderSystem& t_renderSystem);
	~AchievementScreen();

	void update(float t_deltaTime);
	void render(SDL_Renderer* t_renderer);
	void reset();
	void initialise(SDL_Renderer* t_renderer, Controller& t_controller);
private:
	EventManager& m_eventManager;
 
	void setControllerButtonMaps();

	void createBackground(SDL_Renderer* t_renderer);
	void createInputEntity(Controller& t_controller);
	void createAchievementEntities(SDL_Renderer* t_renderer);
	void createCheckBoxEntities(SDL_Renderer* t_renderer);

	void buttonPressed(const MenuButtonPressed& t_event);

	void updateAchievement(const UpdateAchievement& t_event);

	InputSystem& m_inputSystem;
	CommandSystem& m_commandSystem;
	RenderSystem& m_renderSystem;
};