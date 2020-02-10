#include "stdafx.h"
#include "Game.h"

/// <summary>
/// Constructor for the game class.
/// </summary>

class State;
Game::Game() :
	m_tileSize(64),
	m_levelHeight(20),
	m_levelWidth(30),
	m_framesPerSecond(60),
	m_ticksPerSecond(60),
	m_lastTick(0),
	m_lastRender(0),
	m_timePerFrame(0),
	m_timePerTick(0)
{
	try
	{
		m_timePerFrame = 1000 / m_framesPerSecond;
		m_timePerTick = 1000 / m_ticksPerSecond;
		// Try to initalise SDL in general
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) throw "Error Loading SDL";

		// Create SDL Window Centred in Middle Of Screen
		m_window = SDL_CreateWindow("ARGO", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Utilities::SCREEN_WIDTH, Utilities::SCREEN_HEIGHT, NULL);
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
		//add components to player
		for (auto& player : m_players)
		{
			player.addComponent(new HealthComponent(10, 10));
			player.addComponent(new TransformComponent());
			player.addComponent(new InputComponent());
			player.addComponent(new ForceComponent());
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
	m_lastTick = SDL_GetTicks();
	m_lastRender = SDL_GetTicks();
	while (m_isRunning)
	{
		Uint32 currentTick = SDL_GetTicks();
		Uint16 deltaTime = currentTick - m_lastTick;
		Uint16 renderTime = currentTick - m_lastRender;

		bool canRender = checkCanRender(renderTime);
		bool canTick = checkCanTick(deltaTime);

		processEvent();
		for (auto& entity : m_entities)
		{
			if (canTick)
			{
				m_inputSystem.update(entity);
				m_hpSystem.update(entity);
				m_aiSystem.update(entity);
				m_transformSystem.update(entity);
			}
			if (canRender)
			{
				m_renderSystem.render(m_renderer, entity);
			}
		}
		for (auto& entity : m_levelTiles)
		{
			if (canTick)
			{
				m_inputSystem.update(entity);
				m_hpSystem.update(entity);
				m_aiSystem.update(entity);
				m_transformSystem.update(entity);
			}
			if (canRender)
			{
				m_renderSystem.render(m_renderer, entity);
			}
		}
		for (auto& player : m_players)
		{
			if (canTick)
			{
				m_inputSystem.update(player);
				m_hpSystem.update(player);
				m_aiSystem.update(player);
				m_transformSystem.update(player);
			}
			if (canRender)
			{
				m_renderSystem.render(m_renderer, player);
			}
		}

		if (canRender) postRender();
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
		break;
	default:
		break;
	}
}

void Game::preRender()
{
	//setting the focus point for the camera.
	glm::vec2 focusPoint = glm::vec2(0, 0);
	for (auto& player : m_players)
	{
		float tempx = static_cast<TransformComponent*>(player.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos().x;
		focusPoint.x += static_cast<TransformComponent*>(player.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos().x;
		focusPoint.y += static_cast<TransformComponent*>(player.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos().y;
	}
	m_renderSystem.setFocus(focusPoint / 4.0f);


	SDL_RenderClear(m_renderer);
}

void Game::postRender()
{
	SDL_RenderPresent(m_renderer);
}

/// <summary>
/// Render function
/// </summary>
void Game::render()
{
	//setting the focus point for the camera.
	glm::vec2 focusPoint = glm::vec2(0,0);
	for (auto& player : m_players)
	{
		float tempx = static_cast<TransformComponent*>(player.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos().x;
		focusPoint.x += static_cast<TransformComponent*>(player.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos().x;
		focusPoint.y += static_cast<TransformComponent*>(player.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos().y;
	}
	m_renderSystem.setFocus(focusPoint / 4.0f);


	SDL_RenderClear(m_renderer);
	//Draw Here
	for (auto& entity : m_entities)
	{
		m_renderSystem.render(m_renderer, entity);
	}
	for (auto& entity : m_levelTiles)
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
	int count = 0; 
	m_levelTiles.reserve(m_levelHeight * m_levelWidth);
	for (int i = 0; i < m_levelHeight; i++)
	{
		for (int j = 0; j < m_levelWidth; j++)
		{
			m_levelTiles.emplace_back();
			m_levelTiles.at(count).addComponent(new TransformComponent(j * m_tileSize, i * m_tileSize, 0));
			m_levelTiles.at(count).addComponent(new VisualComponent("assets//images//Texture.png", m_renderer));
			count++;
		}
	}
}

bool Game::checkCanRender(Uint16 t_renderTime)
{
	if (t_renderTime > m_timePerFrame)
	{
		m_lastRender += m_timePerFrame;
		preRender();
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
