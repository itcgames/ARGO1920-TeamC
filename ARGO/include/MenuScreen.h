#pragma once
#include "Utilities.h"
#include "EventManager.h"
#include "Controller.h"
#include "InputSystem.h"
#include "CommandSystem.h"
#include "RenderSystem.h"
#include "TagComponent.h"
#include "EventManager.h"

enum class MenuButtonType
{
	Play,
	Options,
	Credits,
	Achievements,
	Quit
};

class MenuScreen
{
public:
	MenuScreen(SDL_Renderer* t_renderer, MenuStates& t_currentScreen, EventManager& t_eventManager, Controller t_controller[Utilities::NUMBER_OF_PLAYERS], ButtonCommandMap t_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS][Utilities::NUMBER_OF_PLAYERS]);
	~MenuScreen();
	void update(bool t_canTick, bool t_canRender, Uint16 t_deltaTime);
private:
 

	MenuStates& m_currentScreen;
	SDL_Renderer* m_renderer;
	EventManager& m_eventManager;

	static const int NUMBER_OF_MENU_BUTTONS = 5;
	Entity m_menuButtons[NUMBER_OF_MENU_BUTTONS];

	Controller m_controllers[Utilities::NUMBER_OF_PLAYERS];
	
	RenderSystem m_renderSystem;
	InputSystem m_inputSystem;
	CommandSystem m_commandSystem;
	 


	// Button Maps
	ButtonCommandMap m_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS][Utilities::NUMBER_OF_PLAYERS];

	MenuButtonType m_currentButton;

	void changeCurrentSelectedButton(const MenuMoveButtonsUpDown& t_event);
	void currentButtonPressed(const MenuSelectButton& t_event);

};

