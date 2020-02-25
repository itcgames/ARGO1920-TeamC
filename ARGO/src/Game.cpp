#include "stdafx.h"
#include "Game.h"



/// <summary>
/// Constructor for the game class.
/// </summary>

class State;
Game::Game() :
	m_gameScreen{ m_renderer, m_eventManager, m_controllers },
	m_optionsScreen{ m_eventManager, m_controllers[0], m_renderer },
	m_creditsScreen{ m_eventManager },
	m_licenseScreen{ m_eventManager },
	m_splashScreen{ m_eventManager },
	m_mainMenuScreen{ m_eventManager },
	m_achievementsScreen{ m_eventManager, m_controllers[0], m_renderer },
	m_currentScreen{ MenuStates::Game }
{
	try
	{
		// Try to initalise SDL in general
		if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER) < 0) throw "Error Loading SDL";

		// Create SDL Window Centred in Middle Of Screen
		m_window = SDL_CreateWindow("ARGO", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Utilities::SCREEN_WIDTH, Utilities::SCREEN_HEIGHT, NULL);

		//initilises mixer, ttf and img
		initLibraries();

		// Check if window was created correctly
		if (!m_window) throw "Error Loading Window";

		//Create the SDL Renderer 
		createRenderer();


		//Check if the renderer was created correctly
		if (!m_renderer) throw "Error Loading Renderer";

		// Sets clear colour of renderer to black and the color of any primitives
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);

		for (int index = 0; index < Utilities::S_MAX_PLAYERS; index++)
		{
			m_controllers[index].initialiseController();
		}
		for (bool hasBeenSet : m_hasScreenBeenSet)
		{
			hasBeenSet = false;
		}

		m_assetMgr = AssetManager::Instance(*m_renderer);
		m_audioMgr = AudioManager::Instance();
		m_audioMgr->PlayMusic("looping\\Ove - Earth Is All We Have.ogg");

		initialiseScreen();
		m_eventManager.subscribeToEvent<CloseWindow>(std::bind(&Game::closeWindow, this, std::placeholders::_1));
		m_eventManager.subscribeToEvent<ChangeScreen>(std::bind(&Game::changeScreen, this, std::placeholders::_1));

		setupIgnoredEvents();

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
	Uint32 timePerFrame = 1000 / 60;
	Uint32 lastTick = SDL_GetTicks();
	Uint32 nextFrame = SDL_GetTicks() + timePerFrame;
	Uint32 currentTick = 0;
	Uint32 timeSinceLastTick = 0;
	while (m_isRunning)
	{
		processEvent();
		while (SDL_GetTicks() <= nextFrame)
		{
			currentTick = SDL_GetTicks();
			timeSinceLastTick = currentTick - lastTick;
			if (timeSinceLastTick > 0)
			{
				processEvent();
				float deltaTime = (float)timeSinceLastTick / (float)timePerFrame;
				lastTick = currentTick;
				update(deltaTime);
			}
		}
		render();
		nextFrame = SDL_GetTicks() + timePerFrame;
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
	while (SDL_PollEvent(&event))
	{
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
			// master volume
			case SDLK_UP:
			{
				m_audioMgr->SetMasterVolume(m_audioMgr->GetMasterVolume() + Utilities::AUDIO_VOLUME_STEP);
				break;
			}
			case SDLK_DOWN:
			{
				m_audioMgr->SetMasterVolume(m_audioMgr->GetMasterVolume() - Utilities::AUDIO_VOLUME_STEP);
				break;
			}
			//sfx volume
			case SDLK_KP_7:
			{
				m_audioMgr->SetSfxVolume(m_audioMgr->GetSfxVolume() + Utilities::AUDIO_VOLUME_STEP);
				break;
			}
			case SDLK_KP_4:
			{
				m_audioMgr->SetSfxVolume(m_audioMgr->GetSfxVolume() - Utilities::AUDIO_VOLUME_STEP);
				break;
			}
			//music volume
			case SDLK_KP_9:
			{
				m_audioMgr->SetMusicVolume(m_audioMgr->GetMusicVolume() + Utilities::AUDIO_VOLUME_STEP);
				break;
			}
			case SDLK_KP_6:
			{
				m_audioMgr->SetMusicVolume(m_audioMgr->GetMusicVolume() - Utilities::AUDIO_VOLUME_STEP);
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
	}
	if (MenuStates::Game == m_currentScreen)
	{
		m_gameScreen.processEvents(&event);
	}
}

void Game::update(float t_dt)
{
	switch (m_currentScreen)
	{
	case MenuStates::Game:
		m_gameScreen.update(t_dt);
		break;
	case MenuStates::MainMenu:
		m_mainMenuScreen.update(t_dt);
		break;
	case MenuStates::Credits:
		m_creditsScreen.update(t_dt);
		break;
	case MenuStates::Options:
		m_optionsScreen.update(t_dt);
		break;
	case MenuStates::License:
		m_licenseScreen.update(t_dt);
		break;
	case MenuStates::Splash:
		m_splashScreen.update(t_dt);
		break;
	case MenuStates::Achievements:
		m_achievementsScreen.update(t_dt);
		break;
	default:
		break;
	}
}

void Game::render()
{
	SDL_RenderClear(m_renderer);
	switch (m_currentScreen)
	{
	case MenuStates::Game:
		m_gameScreen.render(m_renderer);
		break;
	case MenuStates::MainMenu:
		m_mainMenuScreen.render(m_renderer);
		break;
	case MenuStates::Credits:
		m_creditsScreen.render(m_renderer);
		break;
	case MenuStates::Options:
		m_optionsScreen.render(m_renderer);
		break;
	case MenuStates::License:
		m_licenseScreen.render(m_renderer);
		break;
	case MenuStates::Splash:
		m_splashScreen.render(m_renderer);
		break;
	case MenuStates::Achievements:
		m_achievementsScreen.render(m_renderer);
		break;
	default:
		break;
	}
	SDL_RenderPresent(m_renderer);
}

/// <summary>
/// Cleans up after running by deleting stuff
/// </summary>
void Game::cleanup()
{
	AssetManager::Release();
	m_assetMgr = NULL;
	AudioManager::Release();
	m_audioMgr = NULL;
	IMG_Quit();
	TTF_Quit();
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	m_renderer = NULL;
	SDL_Quit();
}

void Game::closeWindow(const CloseWindow& t_event)
{
	m_isRunning = false;
}

void Game::createButtonMaps()
{
	using ButtonCommandPair = std::pair<ButtonType, Command*>;
	for (int index = 0; index < Utilities::S_MAX_PLAYERS; index++)
	{
		m_controllerButtonMaps[static_cast<int>(ButtonState::Pressed)][index].clear();
		m_controllerButtonMaps[static_cast<int>(ButtonState::Pressed)][index] =
		{
			ButtonCommandPair(ButtonType::DpadUp, new MoveUpCommand()),
			ButtonCommandPair(ButtonType::DpadDown, new MoveDownCommand()),
			ButtonCommandPair(ButtonType::DpadLeft, new MoveLeftCommand()),
			ButtonCommandPair(ButtonType::DpadRight, new MoveRightCommand()),
			ButtonCommandPair(ButtonType::Back, new CloseWindowCommand()),
			ButtonCommandPair(ButtonType::RightTrigger, new FireBulletCommand())
		};
		// Set Held To Same as Pressed Commands For Time Being
		m_controllerButtonMaps[static_cast<int>(ButtonState::Held)][index] = m_controllerButtonMaps[static_cast<int>(ButtonState::Pressed)][index];
		// Set Release Commands to nothing
		m_controllerButtonMaps[static_cast<int>(ButtonState::Released)][index] = ButtonCommandMap();
	}
}

void Game::changeScreen(const ChangeScreen& t_event)
{
	m_currentScreen = t_event.newScreen;
	if (m_hasScreenBeenSet[static_cast<int>(m_currentScreen)])
	{
		resetScreen();
	}
	else
	{
		initialiseScreen();
	}
}

void Game::initialiseScreen()
{
	createButtonMaps();
	switch (m_currentScreen)
	{
	case MenuStates::Game:
		m_gameScreen.initialise(m_renderer, m_controllerButtonMaps, m_controllers);
		break;
	case MenuStates::MainMenu:
		m_mainMenuScreen.initialise(m_renderer, m_controllers[0]);
		break;
	case MenuStates::Credits:
		m_creditsScreen.initialise(m_renderer, m_controllers[0]);
		break;
	case MenuStates::Options:
		m_optionsScreen.initialise();
		break;
	case MenuStates::License:
		m_licenseScreen.initialise(m_renderer, m_controllers[0]);
		break;
	case MenuStates::Splash:
		m_splashScreen.initialise(m_renderer, m_controllers[0]);
		break;
	case MenuStates::Achievements:
		m_achievementsScreen.initialise();
		break;
	default:
		break;
	}
	m_hasScreenBeenSet[static_cast<int>(m_currentScreen)] = true;
}

void Game::resetScreen()
{
	createButtonMaps();
	switch (m_currentScreen)
	{
	case MenuStates::Game:
		m_gameScreen.reset(m_renderer, m_controllers);
		break;
	case MenuStates::MainMenu:
		m_mainMenuScreen.reset();
		break;
	case MenuStates::Credits:
		m_creditsScreen.reset();
		break;
	case MenuStates::Options:
		m_optionsScreen.reset();
		break;
	case MenuStates::License:
		m_licenseScreen.reset();
		break;
	case MenuStates::Splash:
		m_splashScreen.reset();
		break;
	case MenuStates::Achievements:
		m_achievementsScreen.reset();
		break;
	default:
		break;
	}
}

void Game::setupIgnoredEvents()
{
	SDL_EventState(SDL_CONTROLLERAXISMOTION, SDL_IGNORE);
	SDL_EventState(SDL_CONTROLLERBUTTONDOWN, SDL_IGNORE);
	SDL_EventState(SDL_CONTROLLERBUTTONUP, SDL_IGNORE);
	SDL_EventState(SDL_CONTROLLERDEVICEADDED, SDL_IGNORE);
	SDL_EventState(SDL_CONTROLLERDEVICEREMOVED, SDL_IGNORE);
	SDL_EventState(SDL_CONTROLLERDEVICEREMAPPED, SDL_IGNORE);
}

void Game::createRenderer()
{
	//create renderer with vsync and hardware rendering in DEBUG mode
#ifdef _DEBUG
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
#else
	//when in release, make window fullscreen
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
	bool IsFullscreen = SDL_GetWindowFlags(m_window) & FullscreenFlag;
	SDL_SetWindowFullscreen(m_window, IsFullscreen ? 0 : FullscreenFlag);
	SDL_ShowCursor(IsFullscreen);
#endif // DEBUG
}
