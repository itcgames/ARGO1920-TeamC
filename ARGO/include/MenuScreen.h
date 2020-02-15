#pragma once
#include "Utilities.h"
#include "Entity.h"
#include "InputSystem.h"
#include "PhysicsSystem.h"
#include "CommandSystem.h"
#include "TagComponent.h"
#include "RenderSystem.h"

class MenuScreen
{
public:
	MenuScreen(MenuStates* t_currentScreen);
	~MenuScreen();
	void update(bool t_canTick, bool t_canRender, Uint16 t_deltaTime);
private:


	void createButton(Entity& t_button);



	MenuStates* m_currentScreen;

	static const int NUMBER_OF_BUTTONS = 4;
	Entity* m_buttons[NUMBER_OF_BUTTONS];

	InputSystem m_inputSystem;
	CommandSystem m_commandSystem;
	RenderSystem m_renderSystem;

	SDL_Renderer* m_renderer;
	MenuStates* m_currentScreen;
	EventManager& m_eventManager;

};

