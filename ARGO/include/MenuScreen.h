#pragma once
#include "Utilities.h"
#include "EventManager.h"
#include "Controller.h"
#include "InputSystem.h"
#include "CommandSystem.h"
#include "RenderSystem.h"
#include "TagComponent.h"
#include "EventManager.h"
#include "PhysicsSystem.h"

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

	MenuScreen(EventManager& t_eventManager, CommandSystem& t_commandSystem, InputSystem& t_input, RenderSystem& t_renderSystem);
	~MenuScreen();
	void update(float t_deltaTime);
	void reset();
	void render(SDL_Renderer* t_renderer);
	void initialise(SDL_Renderer* t_renderer, Controller& t_controller);
 private:
 
	void setControllerButtonMaps();
	void createMenuButton(Entity& t_menuButton, glm::vec2 t_position);
	void createInputEntity(Controller& t_controller);
	void changeCurrentSelectedButton(const MenuMoveBetweenUI& t_event);
	void buttonPressed(const MenuButtonPressed& t_event);
	void updateButtonColour(Entity& t_menuButton, glm::vec3 t_colour);

	static const int NUMBER_OF_MENU_BUTTONS = 5; 

	Entity m_menuButtons[NUMBER_OF_MENU_BUTTONS];
	Entity m_background;
	Entity m_inputEntity;

	MenuButtonType m_currentButton;
 
	ButtonCommandMap m_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS];

	EventManager& m_eventManager;
	InputSystem& m_inputSystem;
	CommandSystem& m_commandSystem;
	RenderSystem& m_renderSystem;

	bool m_screenActive = false;
};

