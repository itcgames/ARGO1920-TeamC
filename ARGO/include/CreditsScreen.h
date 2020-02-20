#pragma once
#include "EventManager.h"
#include "Controller.h"
#include "InputSystem.h"
#include "CommandSystem.h"
#include "RenderSystem.h"

class CreditsScreen
{
public:
	CreditsScreen(EventManager& t_eventManager, Controller& t_controller, SDL_Renderer* t_renderer);
	~CreditsScreen();

	void update(Uint16 t_deltaTime);
	void render(SDL_Renderer* t_renderer);
	void reset();
private:
	void setControllerButtonMaps();

	Entity m_inputEntity;
	Entity m_bachground;

	RenderSystem m_renderSystem;
	CommandSystem m_commandSystem;
	InputSystem m_inputSystem;

	ButtonCommandMap m_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS];
	EventManager& m_eventManager;
 };

