#include "stdafx.h"
#include "Game.h"

/// <summary>
/// Constructor for the game class.
/// </summary>
Game::Game()
{
	try
	{
		// Try to initalise SDL in general
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) throw "Error Loading SDL";

		// Create SDL Window Centred in Middle Of Screen
		m_window = SDL_CreateWindow("Final Year Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, NULL);
		// Check if window was created correctly
		if (!m_window) throw "Error Loading Window";

		//Create the SDL Renderer 
		m_renderer = SDL_CreateRenderer(m_window, -1, 0);
		//Check if the renderer was created correclty
		if (!m_renderer) throw "Error Loading Renderer";

		// Sets clear colour of renderer to black and the color of any primitives
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
		// Game is running
		m_isRunning = true;

		//add components to player
		for (auto& player : m_players)
		{
			player.addComponent(new HealthComponent(10, 10));
			player.addComponent(new TransformComponent());
			player.addComponent(new InputComponent());
		}

		m_entities.reserve(1000);
		for (int i = 0; i < 5; i++)
		{
			m_entities.emplace_back();
			m_entities.at(i).addComponent(new TransformComponent());
		}
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
		if (SDLK_SPACE == event.key.keysym.sym)
		{

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
	for (auto& entity : m_entities)
	{
		if (entity.hasComponentType(ComponentType::Health))
		{
			m_hpSystem.update(entity);
		}	
		if (entity.hasComponentType(ComponentType::Input))
		{
			m_inputSystem.update(entity);
		}
		if (entity.hasComponentType(ComponentType::Ai))
		{
			m_aiSystem.update(entity);
		}
		if (entity.hasComponentType(ComponentType::Transform))
		{
			m_transformSystem.update(entity);
		}
	}
	for (auto& player : m_players)
	{
		if (player.hasComponentType(ComponentType::Health))
		{
			m_hpSystem.update(player);
		}
		if (player.hasComponentType(ComponentType::Input))
		{
			m_inputSystem.update(player);
		}
		if (player.hasComponentType(ComponentType::Ai))
		{
			m_aiSystem.update(player);
		}
		if (player.hasComponentType(ComponentType::Transform))
		{
			m_transformSystem.update(player);
		}
	}
}

/// <summary>
/// Render function
/// </summary>
void Game::render()
{
	SDL_RenderClear(m_renderer);

	//Draw Here
	for (auto& entity : m_entities)
	{
		if (entity.hasComponentType(ComponentType::Transform) || entity.hasComponentType(ComponentType::Visual))
		{
			m_renderSystem.render(m_renderer, entity);
		}
	}
	for (auto& player : m_players)
	{
		if (player.hasComponentType(ComponentType::Transform) || player.hasComponentType(ComponentType::Visual))
		{
			m_renderSystem.render(m_renderer, player);
		}
	}

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
