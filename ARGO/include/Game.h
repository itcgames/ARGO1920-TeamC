#pragma once
#include "AssetManager.h"
#include "EventManager.h"  
#include "AudioManager.h" 
#include "GameScreen.h"
#include "MenuScreen.h"
#include "CreditsScreen.h"
#include "OptionsScreen.h"
#include "SplashScreen.h"
#include "LicenseScreen.h"

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

	void initLibraries();
	void processEvent();
	void update(bool t_canTick, bool t_canRender, Uint16 t_dt);
 	void cleanup(); 
	bool checkCanRender(Uint16 t_currentTick);
	bool checkCanTick(Uint16 t_currentTick);
	void closeWindow(const CloseWindow& t_event = CloseWindow());
	void createButtonMaps();



	AssetManager* m_assetMgr;
	AudioManager* m_audioMgr;

	EventManager m_eventManager;


	// Window used for the game
	SDL_Window* m_window;
	// Renderer used to render onto screen
	SDL_Renderer* m_renderer;

	// bool for if game is running or not
	bool m_isRunning;

	Uint16 m_timePerFrame;
	Uint16 m_timePerTick;
	Uint16 m_lastTick;
	Uint16 m_lastRender;
	Uint16 m_framesPerSecond;
	Uint16 m_ticksPerSecond;


 
	MenuStates m_currentScreen;
	GameScreen* m_gameScreen; 
	MenuScreen* m_mainMenuScreen;
	OptionsScreen* m_optionsScreen;
	CreditsScreen* m_creditsScreen;
	LicenseScreen* m_licenseScreen;
	SplashScreen* m_splashScreen;

	void initialiseScreens();



	
	Controller m_controllers[Utilities::NUMBER_OF_PLAYERS];



	ButtonCommandMap m_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS][Utilities::NUMBER_OF_PLAYERS];
 

};