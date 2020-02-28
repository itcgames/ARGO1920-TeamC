#pragma once
#include "EventManager.h"
#include "Controller.h"
#include "InputSystem.h"
#include "CommandSystem.h"
#include "RenderSystem.h"

class CreditsScreen
{
public:
	CreditsScreen(EventManager& t_eventManager, CommandSystem& t_commandSystem, InputSystem& t_input, RenderSystem& t_renderSystem);
	~CreditsScreen();

	void update(float t_deltaTime);
	void render(SDL_Renderer* t_renderer);
	void reset();
	void initialise(SDL_Renderer* t_renderer, Controller& t_controller);
 private:
	void setControllerButtonMaps();
	void returnToMainMenu(const MenuButtonPressed& t_event);

	Entity m_inputEntity;
	Entity m_background;

	RenderSystem& m_renderSystem;
	CommandSystem& m_commandSystem;
	InputSystem& m_inputSystem;

	ButtonCommandMap m_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS];
	EventManager& m_eventManager;
};