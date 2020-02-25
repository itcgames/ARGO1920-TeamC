#pragma once
#include "EventManager.h"
#include "InputSystem.h"
#include "RenderSystem.h"
#include "Controller.h"
#include "Utilities.h"
#include "CommandSystem.h" 

enum class GameTypeButtonType
{
	Offline,
	OnlineHost,
	OnlineJoin
};

enum class GameTypeTitles
{
	Main,
	Offline, 
	Online
};

class GameTypeScreen
{
public:
	GameTypeScreen(EventManager& t_eventManager, CommandSystem& t_commandSystem, InputSystem& t_inputSystem, RenderSystem& t_renderSystem);
	~GameTypeScreen();
	void update(float t_deltaTime);
	void reset();
	void render(SDL_Renderer* t_renderer);
	void initalise(SDL_Renderer* t_renderer, Controller& t_controller);
 private:

	void setControllerButtonMaps();
	void createGameTypeButton(Entity& t_gameTypeButton, glm::vec2 t_position);
	void createInputEntity(Controller& t_controller);
	void changeCurrentSelectedButton(const GameTypeMoveButtonsUpDown& t_event);
	void updateButtonColour(Entity& t_gameTypeButton, glm::vec3 t_colour);
	void buttonPressed(const GameTypeSelectButton& t_event);

	static const int S_NUMBER_OF_GAME_TYPE_BUTTONS = 3; 
	static const int S_NUMBER_OF_GAME_TYPE_TITLES = 3;

	Entity m_gameTypeButtons[S_NUMBER_OF_GAME_TYPE_BUTTONS];
	Entity m_gameTypeTitles[S_NUMBER_OF_GAME_TYPE_TITLES];
	Entity m_inputEntity;
	Entity m_background;

	Entity m_hostPopup;

	GameTypeButtonType m_currentButton;

	ButtonCommandMap m_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS];

	EventManager& m_eventManager;
	InputSystem& m_inputSystem;
	CommandSystem& m_commandSystem;
	RenderSystem& m_renderSystem;
};

