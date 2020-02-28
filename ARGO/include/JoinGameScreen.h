#pragma once
#include "EventManager.h"
#include "InputSystem.h"
#include "RenderSystem.h"
#include "Utilities.h"
#include "Controller.h"
#include "CommandSystem.h"

enum class PlayerJoinedParts
{
	Border,
	Player,
	PlaceHolder,
	Text
};

class JoinGameScreen
{
public:
	JoinGameScreen(EventManager& t_eventManager, CommandSystem& t_commandSystem, InputSystem& t_inputSystem, RenderSystem& t_renderSystem, Controller(&t_controllers)[Utilities::S_MAX_PLAYERS]);
	~JoinGameScreen();
	void update(float t_deltaTime);
	void reset();
	void render(SDL_Renderer* t_renderer);
	void initialise(SDL_Renderer* t_renderer);
	void playerHasJoined();
	void setIsActive(bool t_isActive);
	bool getIsActive();
private:

	void setControllerButtonMaps();

	void buttonPressed(const Events::MenuButtonPressed& t_event);
	void startGame();
	void cancel();

	void createInputEntities();
	void createBackground(SDL_Renderer* t_renderer);
	void createPlayerJoinedEntities(SDL_Renderer* t_renderer);
	void createHelpText(SDL_Renderer* t_renderer);
	void findHostsIp();

	void playersJoin(const Events::JoinGame& t_event);

	std::string m_hostsIp;

	Entity m_background;
	Entity m_inputEntities[Utilities::S_MAX_PLAYERS];

	static const int S_NUMBER_OF_PLAYER_JOINED_ENTITIES_PARTS = 4;
	Entity m_playerJoinedEntity[Utilities::S_MAX_PLAYERS][S_NUMBER_OF_PLAYER_JOINED_ENTITIES_PARTS];

	Entity m_helpText[2];

	bool m_playersHaveJoined[Utilities::S_MAX_PLAYERS];

	EventManager& m_eventManager;
	InputSystem& m_inputSystem;
	CommandSystem& m_commandSystem;
	RenderSystem& m_renderSystem;

	ButtonCommandMap m_controllerButtonMaps[Utilities::S_MAX_PLAYERS];

	bool m_screenActive = false;

	Controller(&m_controllers)[Utilities::S_MAX_PLAYERS];
	Controller m_joinedControllers[Utilities::S_MAX_PLAYERS];
};

