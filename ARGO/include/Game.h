#pragma once
#include "EventManager.h"  
#include "AudioManager.h" 
#include "GameScreen.h"
#include "MenuScreen.h"
#include "CreditsScreen.h"
#include "OptionsScreen.h"
#include "SplashScreen.h"
#include "LicenseScreen.h"
#include "AchievementScreen.h"
#include "GameTypeScreen.h"
#include "JoinGameScreen.h"
#include "Event.h"

#include "AiSystem.h"
#include "CollisionSystem.h"
#include "CommandSystem.h"
#include "HealthSystem.h"
#include "InputSystem.h"
#include "ParticleSystem.h"
#include "PhysicsSystem.h"
#include "RenderSystem.h"

#include "OnlineGameHandler.h"

/// <summary>
/// Game class needed for the game
/// </summary>
class Game
{
public:
	Game();
	~Game();
	void run();
private:
	void update(float t_deltaTime);
	void initLibraries();
	void processEvent();
	void initialiseScreen();
	void resetScreen();
 	void createButtonMaps();
	void setupIgnoredEvents();
	void createRenderer();
	void render();
	void cleanup();
	void killGameServer();

	void closeWindow(const Events::CloseWindow& t_event = Events::CloseWindow());
	void changeScreen(const Events::ChangeScreen& t_event);
	void setProperControllers(const Events::SetControllers& t_event);

	AssetManager* m_assetMgr;
	AudioManager* m_audioMgr;
	EventManager m_eventManager;

	// Window used for the game
	SDL_Window* m_window;
	// Renderer used to render onto screen
	SDL_Renderer* m_renderer;

	// bool for if game is running or not
	bool m_isRunning;

	//ECS systems
 	CommandSystem m_commandSystem;
 	InputSystem m_inputSystem;
 	RenderSystem m_renderSystem;

	//Screens and states
	MenuStates m_currentScreen;
	GameScreen m_gameScreen; 
	MenuScreen m_mainMenuScreen;
	OptionsScreen m_optionsScreen;
	CreditsScreen m_creditsScreen;
	LicenseScreen m_licenseScreen;
	SplashScreen m_splashScreen;
	AchievementScreen m_achievementsScreen;
	GameTypeScreen m_gameTypeScreen;
	JoinGameScreen m_joinGameScreen;

	Controller m_controllers[Utilities::S_MAX_PLAYERS];
	Controller m_properControllers[Utilities::S_MAX_PLAYERS];

	ButtonCommandMap m_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS][Utilities::S_MAX_PLAYERS];

	bool m_hasScreenBeenSet[Utilities::S_NUMBER_OF_SCREENS];

	OnlineGameHandler m_onlineHandler;
	
 };