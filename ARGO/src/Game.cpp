#include "stdafx.h"
#include "Game.h"

/// <summary>
/// Constructor for the game class.
/// </summary>
class State;
Game::Game() :
	m_tileSize(20),
	m_levelHeight(100),
	m_levelWidth(100)
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
			player.addComponent(new ColourComponent(glm::linearRand(0, 255), glm::linearRand(0, 255), glm::linearRand(0, 255), 255));
		}

		m_entities.reserve(MAX_ENTITIES);
		for (int i = 0; i < 5; i++)
		{
			m_entities.emplace_back();
			m_entities.at(i).addComponent(new TransformComponent());
			m_entities.at(i).addComponent(new AiComponent());
		}

		setupLevel();
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
			if (m_entities.size() > 0)
			{
				m_entities.erase(--m_entities.end());
			}
			std::cout << m_entities.size() << std::endl;
		}
		if (SDLK_BACKSPACE == event.key.keysym.sym)
		{
			//delete all entities
			if (m_entities.size() > 0)
			{
				m_entities.erase(m_entities.begin(), m_entities.end());
			}
			std::cout << m_entities.size() << std::endl;
		}
		if (SDLK_DELETE == event.key.keysym.sym)
		{
			//delete all entities
			m_players[0].removeCompType(ComponentType::Input);
		}
		if (SDLK_RETURN == event.key.keysym.sym)
		{
			//check if we can add 100 entities
			int availableSpace = MAX_ENTITIES - m_entities.size();
			//if more than 100 available, set to 100
			if (availableSpace > 100)
			{
				availableSpace = 100;
			}
			//if at least 1 available
			if (availableSpace > 0)
			{
				for (int i = 0; i < availableSpace; i++)
				{
					m_entities.emplace_back();
					m_entities.at(m_entities.size() - 1).addComponent(new TransformComponent());
					m_entities.at(m_entities.size() - 1).addComponent(new AiComponent());
				}
			}
			std::cout << m_entities.size() << std::endl;
		}
		if (SDLK_1 == event.key.keysym.sym)
		{
			//if space available in the vector
			if (m_entities.size() < MAX_ENTITIES)
			{
				//add one
				m_entities.emplace_back();
				m_entities.at(m_entities.size() - 1).addComponent(new TransformComponent());
				m_entities.at(m_entities.size() - 1).addComponent(new AiComponent());
			}
			std::cout << m_entities.size() << std::endl;
		}

		if (SDLK_UP == event.key.keysym.sym)
		{
			m_fsm.idle();
		}
		if (SDLK_DOWN == event.key.keysym.sym)
		{
			m_fsm.moving();
		}
		if (SDLK_LEFT == event.key.keysym.sym)
		{
			m_fsm.attacking();
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
		m_hpSystem.update(entity);
		m_inputSystem.update(entity);
		m_aiSystem.update(entity);
		m_transformSystem.update(entity);
	}
	for (auto& player : m_players)
	{
		m_hpSystem.update(player);
		m_inputSystem.update(player);
		m_aiSystem.update(player);
		m_transformSystem.update(player);
	}

	m_fsm.update();
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
		m_renderSystem.render(m_renderer, entity);
	}
	for (auto& player : m_players)
	{
		m_renderSystem.render(m_renderer, player);
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

void Game::setupLevel()
{
	m_levelTiles.clear();
	for (int i = 0; i < m_levelHeight; i++)
	{
		for (int j = 0; j < m_levelWidth; j++)
		{
			m_levelTiles.push_back(Tile(glm::vec2(i, j), m_tileSize, TileType::Ground));
		}
	}
}
