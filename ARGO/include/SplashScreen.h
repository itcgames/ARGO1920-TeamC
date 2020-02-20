#pragma once
#include "EventManager.h"
#include "Controller.h"
#include "InputSystem.h"
#include "CommandSystem.h"
#include "RenderSystem.h"

class SplashScreen
{
public:
	SplashScreen(EventManager& t_eventManager, Controller& t_controller, SDL_Renderer* t_renderer);
	~SplashScreen();

	void update(Uint16 t_deltaTime);
	void render(SDL_Renderer* t_renderer);
	void reset();
private:
	void setControllerButtonMaps();

	Entity m_inputEntity;
	Entity m_background;

	RenderSystem m_renderSystem;
	CommandSystem m_commandSystem;
	InputSystem m_inputSystem;

	ButtonCommandMap m_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS];
	EventManager& m_eventManager;

	Uint32 m_screenStartTime;
	static const Uint32 S_TIME_ON_SCREEN = 2000;
 };