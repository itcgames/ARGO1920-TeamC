#include "stdafx.h"
#include "Game.h"

/// <summary>
/// Constructor for the game class.
/// </summary>

class State;
Game::Game() :
	m_framesPerSecond(60),
	m_ticksPerSecond(60),
	m_lastTick(0),
	m_lastRender(0),
	m_timePerFrame(0),
	m_timePerTick(0)
{
	try
	{ 
		// Try to initalise SDL in general
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) throw "Error Loading SDL";

		// Create SDL Window Centred in Middle Of Screen
		m_window = SDL_CreateWindow("ARGO", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Utilities::SCREEN_WIDTH, Utilities::SCREEN_HEIGHT, NULL);

		//initilises mixer, ttf and img
		initLibraries();

		// Check if window was created correctly
		if (!m_window) throw "Error Loading Window";

		//Create the SDL Renderer 
		m_renderer = SDL_CreateRenderer(m_window, -1, 0);
		//Check if the renderer was created correctly
		if (!m_renderer) throw "Error Loading Renderer";

		// Sets clear colour of renderer to black and the color of any primitives
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);

		m_assetMgr = AssetManager::Instance(*m_renderer);
		m_audioMgr = AudioManager::Instance();
		initialiseScreens();


		m_eventManager.subscribeToEvent<CloseWindow>(std::bind(&Game::closeWindow, this, std::placeholders::_1));
		m_timePerFrame = 1000 / m_framesPerSecond;
		m_timePerTick = 1000 / m_ticksPerSecond;
		

		/*m_textTest1.addComponent(new TransformComponent());
		m_textTest1.addComponent(new TextComponent("comic.ttf", m_renderer, true, std::string("Static Text")));
		m_textTest2.addComponent(new TransformComponent());
		m_textTest2.addComponent(new TextComponent("pt-sans.ttf", m_renderer, Utilities::LARGE_FONT, false, "Not Static Text", 255, 255, 0, 123));*/	 
	

		// Game is running
		m_isRunning = true;

	}
	catch (std::string error)
	{
		// output error if any part above doesnt load correctly
		std::cout << error << std::endl;
		// game doesnt run
		m_isRunning = false;
	}
}

/// <summary>
/// destructor for the game class
/// </summary>
Game::~Game()
{
	cleanup();
}

/// <summary>
/// function for the main game loop
/// </summary>
void Game::run()
{
	m_lastTick = SDL_GetTicks();
	m_lastRender = SDL_GetTicks();
	while (m_isRunning)
	{
		processEvent();
		Uint32 currentTick = SDL_GetTicks();
		Uint16 deltaTime = currentTick - m_lastTick;
		Uint16 renderTime = currentTick - m_lastRender;

		bool canRender = checkCanRender(renderTime);
		bool canTick = checkCanTick(deltaTime);
		update(canRender, canTick, deltaTime);

		if (canRender)
		{
			SDL_RenderPresent(m_renderer);
		}
	}
}

void Game::initLibraries()
{
	//Initialize SDL_TTF
	if (TTF_Init() < 0)
	{
		printf("SDL_TTF could not initialize! SDL_TTF Error: %s\n", TTF_GetError());
	}

	if ((IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}
}

/// <summary>
/// Processes any SDL event
/// </summary>
void Game::processEvent()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		closeWindow();
		break;
	case SDL_KEYDOWN:
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
		{
			closeWindow();
			break;
		}
		case SDLK_q:
		{
			m_audioMgr->PlaySfx("airhorn.wav");
			break;
		}
		case SDLK_UP:
		{
			m_audioMgr->SetVolume(m_audioMgr->GetVolume() + 10);
			break;
		}
		case SDLK_DOWN:
		{
			m_audioMgr->SetVolume(m_audioMgr->GetVolume() - 10);
			break;
		}
		default:
			break;
		}
		break;
	} 
	default:
		break;
	}

	if (MenuStates::Game == m_currentScreen)
	{
		m_gameScreen->processEvents(&event);
	}
}

void Game::update(bool t_canTick, bool t_canRender, Uint16 t_dt)
{
	switch (m_currentScreen)
	{
	case MenuStates::Game:
		m_gameScreen->update(t_canTick, t_canRender, t_dt);
		break;
	case MenuStates::MainMenu:
		m_mainMenuScreen->update(t_canTick, t_canRender, t_dt);
		break;
	case MenuStates::Credits:
		m_creditsScreen->update(t_canTick, t_canRender, t_dt);
		break;
	case MenuStates::Options:
		m_optionsScreen->update(t_canTick, t_canRender, t_dt);
		break;
	case MenuStates::License:
		m_licenseScreen->update(t_canTick, t_canRender, t_dt);
		break;
	case MenuStates::Splash:
		m_splashScreen->update(t_canTick, t_canRender, t_dt);
		break;
	default:
		break;
	}
	/*if (t_canRender)
	{
		m_renderSystem.render(m_renderer, m_textTest1);
		m_renderSystem.render(m_renderer, m_textTest2);
	}*/
}
 

/// <summary>
/// Cleans up after running by deleting stuff
/// </summary>
void Game::cleanup()
{
	AudioManager::Release();
	m_audioMgr = NULL;
	AssetManager::Release();
	m_assetMgr = NULL;
	IMG_Quit();
	Mix_Quit();
	TTF_Quit();
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	m_renderer = NULL;
	SDL_Quit();
}  

bool Game::checkCanRender(Uint16 t_renderTime)
{
	if (t_renderTime > m_timePerFrame)
	{
		m_lastRender += m_timePerFrame;
		SDL_RenderClear(m_renderer);
		return true;
	}
	return false;
}

bool Game::checkCanTick(Uint16 t_deltaTime)
{
	if (t_deltaTime > m_timePerTick)
	{
		m_lastTick += m_timePerTick;
		return true;
	}
	return false;
}

void Game::closeWindow(const CloseWindow& t_event)
{
	m_isRunning = false;
}

void Game::initialiseScreens()
{
	m_gameScreen = new GameScreen(m_renderer, &m_currentScreen, m_eventManager);
	m_optionsScreen = new OptionsScreen(&m_currentScreen);
	m_creditsScreen = new CreditsScreen(&m_currentScreen);
	m_licenseScreen = new LicenseScreen(&m_currentScreen);
	m_splashScreen = new SplashScreen(&m_currentScreen);
	m_mainMenuScreen = new MenuScreen(&m_currentScreen);
	m_currentScreen = MenuStates::Game;
}
