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
	Text
};

class JoinGameScreen
{
public:
	JoinGameScreen(EventManager& t_eventManager, CommandSystem& t_commandSystem, InputSystem& t_inputSystem, RenderSystem& t_renderSystem);
	~JoinGameScreen();
	void update(float t_deltaTime);
	void reset();
	void render(SDL_Renderer* t_renderer);
	void initialise(SDL_Renderer* t_renderer, Controller& t_controller);
	void playerHasJoined();
private:

	void setControllerButtonMaps();

	void buttonPressed(const MenuButtonPressed& t_event);
	void startGame();
	void cancel();

	void createInputEntity(Controller& t_controller);
	void createBackground(SDL_Renderer* t_renderer);
	void createPlayerJoinedEntities(SDL_Renderer* t_renderer);
	void createHelpText(SDL_Renderer* t_renderer);

	Entity m_background;
	Entity m_inputEntity;

	static const int S_NUMBER_OF_PLAYER_JOINED_ENTITIES_PARTS = 3;
	Entity m_playerJoinedEntity[Utilities::S_MAX_PLAYERS][S_NUMBER_OF_PLAYER_JOINED_ENTITIES_PARTS];

	Entity m_helpText;

	bool m_playersHaveJoined[Utilities::S_MAX_PLAYERS];

	EventManager& m_eventManager;
	InputSystem& m_inputSystem;
	CommandSystem& m_commandSystem;
	RenderSystem& m_renderSystem;

	ButtonCommandMap m_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS];

	bool m_screenActive = false;
};

