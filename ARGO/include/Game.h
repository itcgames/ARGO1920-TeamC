#pragma once
#include "EventManager.h"  
#include "AudioManager.h" 
#include "GameScreen.h"
#include "MenuScreen.h"
#include "CreditsScreen.h"
#include "HelpScreen.h"
#include "SplashScreen.h"
#include "LicenseScreen.h"
#include "AchievementScreen.h" 
#include "Event.h"

#include "AiSystem.h"
#include "CollisionSystem.h"
#include "CommandSystem.h"
#include "HealthSystem.h"
#include "InputSystem.h"
#include "ParticleSystem.h"
#include "PhysicsSystem.h"
#include "RenderSystem.h"

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

	void closeWindow(const CloseWindow& t_event = CloseWindow());
	void changeScreen(const ChangeScreen& t_event);
 
	int shownAchv = -1;
	int timeSincePopup = -1;

	AssetManager* m_assetMgr;
	AudioManager* m_audioMgr;
	EventManager m_eventManager;

	// Window used for the game
	SDL_Window* m_window;
	// Renderer used to render onto screen
	SDL_Renderer* m_renderer;

	// bool for if game is running or not
	bool m_isRunning;



 	CommandSystem m_commandSystem;
 	InputSystem m_inputSystem;
 	RenderSystem m_renderSystem;


	

	MenuStates m_currentScreen;
	GameScreen m_gameScreen; 
	MenuScreen m_mainMenuScreen;
	HelpScreen m_helpScreen;
	CreditsScreen m_creditsScreen;
	LicenseScreen m_licenseScreen;
	SplashScreen m_splashScreen;
	AchievementScreen m_achievementsScreen; 

	Controller m_controllers[Utilities::S_MAX_PLAYERS];

	ButtonCommandMap m_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS][Utilities::S_MAX_PLAYERS];

	bool m_hasScreenBeenSet[Utilities::S_NUMBER_OF_SCREENS];

 };