#include "Game.h"

/// <summary>
/// Constructor for the game class.
/// </summary>
Game::Game():
	buttonPressedMap{ std::pair<ButtonType, Command*>(ButtonType::A, new APressedCommand()),
					  std::pair<ButtonType, Command*>(ButtonType::B, new XPressedCommand()),
					  std::pair<ButtonType, Command*>(ButtonType::X, new XPressedCommand()),
					  std::pair<ButtonType, Command*>(ButtonType::Y, new YPressedCommand()) }

{
	try
	{
		// Try to initalise SDL in general
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) throw "Error Loading SDL";
		
		// Create SDL Window Centred in Middle Of Screen
		m_window = SDL_CreateWindow("ARGO", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, NULL);
		// Check if window was created correctly
		if (!m_window) throw "Error Loading Window";

		//Create the SDL Renderer 
		m_renderer = SDL_CreateRenderer(m_window, -1, 0);
		//Check if the renderer was created correctly
		if (!m_renderer) throw "Error Loading Renderer";

		// Sets clear colour of renderer to black and the color of any primitives
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
		// Game is running
		m_isRunning = true;
		for (int index = 0; index < SDL_NumJoysticks(); index++)
		{
			controllers.push_back(new Controller());
		} 
		m_inputHandler.setReleasedButtonMap(buttonPressedMap);
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
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	int frameTime;
	while (m_isRunning)
	{
		frameStart = SDL_GetTicks();
		frameTime = SDL_GetTicks() - frameStart;
		processEvent();
		update();
		render();
		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
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
		m_isRunning = false;
		break;
	case SDL_KEYDOWN:
		// Press Escape to close screen
		if (SDLK_ESCAPE == event.key.keysym.sym)
		{
			m_isRunning = false;
		}
		break;
	default:
		break;
	}
}

/// <summary>
/// Update function
/// </summary>
void Game::update()
{
	for (int index = 0; index < SDL_NumJoysticks(); index++)
	{
		controllers[index]->update();
		controllers[index]->getAxisState(AxisType::RightTrigger);
		controllers[index]->getAxisState(AxisType::LeftTrigger);

		controllers[index]->getAxisState(AxisType::LeftThumbStick);
		controllers[index]->getAxisState(AxisType::RightThumbStick);

	}
	m_inputHandler.handleControllerInput(controllers[0]);

	
}

/// <summary>
/// Render function
/// </summary>
void Game::render()
{
	SDL_RenderClear(m_renderer);
	//Draw Here
	SDL_RenderPresent(m_renderer);
}

/// <summary>
/// Cleans up after running by deleting stuff
/// </summary>
void Game::cleanup()
{
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	SDL_QUIT;
}
