#pragma once
 #include "EventManager.h"
#include "InputSystem.h"
#include "RenderSystem.h"
#include "Controller.h"
#include "Utilities.h"
#include "CommandSystem.h" 
#include <iostream>
#include <fstream>
#include "OnlineGameHandler.h"

enum class MenuButtonsType
{
	Offline,
	OnlineHost,
	OnlineJoin
};

enum class MenuTitles
{
	Main,
	Offline,
	Online
};

enum class DialEntityType
{
	Dial,
	Text,
	Arrows
};

class GameTypeScreen
{
public:
	GameTypeScreen(EventManager& t_eventManager, CommandSystem& t_commandSystem, InputSystem& t_inputSystem, RenderSystem& t_renderSystem, OnlineGameHandler& t_onlineHandler);
	~GameTypeScreen();
	void update(float t_deltaTime);
	void reset();
	void render(SDL_Renderer* t_renderer);
	void initialise(SDL_Renderer* t_renderer, Controller& t_controller);
	void setIsActive(bool t_isActive);
	bool getIsActive();
private:
	void attemptToConnect(std::string t_ip, int t_port = Utilities::OnlineData::PORT_NUM);
	void setControllerButtonMaps();
	void updateButtonColour(Entity& t_gameTypeButton, glm::vec3 t_colour);

	void moveThroughUI(const Events::MenuMoveBetweenUI& t_event);
	void buttonPressed(const Events::MenuButtonPressed& t_event);
	void gameTypeConfirmed();
	void gameTypeCancel();
	void gameTypeChosen();

	static const int S_NUMBER_OF_GAME_TYPE_BUTTONS = 3;
	static const int S_NUMBER_OF_GAME_TYPE_TITLES = 3;

	Entity m_gameTypeButtons[S_NUMBER_OF_GAME_TYPE_BUTTONS];
	Entity m_gameTypeTitles[S_NUMBER_OF_GAME_TYPE_TITLES];
	Entity m_inputEntity;
	Entity m_background;

	Entity m_popup;

	static const int S_NUMBER_OF_JOIN_TEXT_LINES = 2;
	Entity m_joinText[2];
	static const int S_IP_NUMBER_LENGTH = 12;
	Entity m_ipNumbers[S_IP_NUMBER_LENGTH][3];
	Entity m_ipDots[3];

	MenuButtonsType m_currentButton;

	ButtonCommandMap m_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS];

	EventManager& m_eventManager;
	InputSystem& m_inputSystem;
	CommandSystem& m_commandSystem;
	RenderSystem& m_renderSystem;

	int m_currentSelectedIpNumber = 0;
	int ipNumberValues[S_IP_NUMBER_LENGTH];

	bool m_joinPopupActive = false;

	void updateIpDial(MoveDirection t_inputDirection);
	void updateCurrentButton(MoveDirection t_inputDirection);

	void createBackgroundEntity(SDL_Renderer* t_renderer);
	void createMenuButtonsAndTitles(SDL_Renderer* t_renderer);
	void createPopupEntity(SDL_Renderer* t_renderer);
	void createInputEntity(Controller& t_controller);
	void createJoinText(SDL_Renderer* t_renderer, glm::vec2 t_popUpPos, float t_popUpHeight);
	void createIpInputter(SDL_Renderer* t_renderer, glm::vec2 t_popupPos);

	void findHostsIp();
	std::string m_hostsIp;

	bool m_screenActive = false;
	STARTUPINFO m_startupInfo = { sizeof(m_startupInfo) };;
	PROCESS_INFORMATION m_processInfo;

	OnlineGameHandler& m_onlineHandler;
};


