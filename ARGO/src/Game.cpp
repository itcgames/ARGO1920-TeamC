#include "stdafx.h"
#include "Game.h"

/// <summary>
/// Constructor for the game class.
/// </summary>

class State;
Game::Game() :
	m_transformSystem{ m_eventManager },
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

		m_eventManager.subscribeToEvent<CloseWindow>(std::bind(&Game::closeWindow, this, std::placeholders::_1));

		//add components to player
		for (auto& player : m_players)
		{
			createPlayer(player);
		}

		m_entities.reserve(MAX_ENTITIES);
		for (int i = 0; i < 5; i++)
		{
			createEnemy();
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
			createBulletEvent bulletEvent{ static_cast<TransformComponent*>(m_players[0].getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos(),
								glm::vec2(50,0),
								0
			};
			m_projectileManager.createPlayerBullet(bulletEvent);
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
					createEnemy();
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

void Game::update(bool t_canTick, bool t_canRender, Uint16 t_dt)
{
	for (auto& entity : m_levelTiles)
	{
		if (t_canTick)
		{
			m_collisionSystem.update(entity);
		}
		if (t_canRender)
		{
			m_renderSystem.render(m_renderer, entity);
		}
	}
	for (auto& entity : m_entities)
	{
		if (t_canTick)
		{
			m_hpSystem.update(entity);
			m_aiSystem.update(entity);
			m_transformSystem.update(entity);
			m_collisionSystem.update(entity);
		}
		if (t_canRender)
		{
			m_renderSystem.render(m_renderer, entity);
		}
	}
	for (auto& player : m_players)
	{
		if (t_canTick)
		{
			m_inputSystem.update(player, m_eventManager);
			m_hpSystem.update(player);
			m_transformSystem.update(player);
			m_collisionSystem.update(player);
		}
		if (t_canRender)
		{
			m_renderSystem.render(m_renderer, player);
		}
	}
	if (t_canTick)
	{
		m_projectileManager.update(&m_transformSystem);
		m_projectileManager.update(&m_collisionSystem);
	}
	if (t_canRender)
	{
		m_projectileManager.render(m_renderer, &m_renderSystem);
	}

	if (t_canTick) m_collisionSystem.handleCollisions();
}

void Game::preRender()
{
	//setting the focus point for the camera.
	glm::vec2 focusPoint = glm::vec2(0, 0);
	for (auto& player : m_players)
	{
		focusPoint.x += static_cast<TransformComponent*>(player.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos().x;
		focusPoint.y += static_cast<TransformComponent*>(player.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos().y;
	}
	m_renderSystem.setFocus(focusPoint / 4.0f);

	SDL_RenderClear(m_renderer);
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
	m_levelTiles.reserve(Utilities::LEVEL_TILE_HEIGHT * Utilities::LEVEL_TILE_WIDTH);
	for (int i = 0; i < Utilities::LEVEL_TILE_HEIGHT; i++)
	{
		for (int j = 0; j < Utilities::LEVEL_TILE_WIDTH; j++)
		{
			m_levelTiles.emplace_back();

			setToFloor(m_levelTiles.at(count), glm::vec2(j * Utilities::TILE_SIZE, i * Utilities::TILE_SIZE));
			count++;
		}
	}
}

void Game::createPlayer(Entity& t_player)
{
	std::map<ButtonType, Command*> buttonPressMap = {
		std::pair<ButtonType, Command*>(ButtonType::DpadUp, new MoveUpCommand()),
		std::pair<ButtonType, Command*>(ButtonType::DpadDown, new MoveDownCommand()),
		std::pair<ButtonType, Command*>(ButtonType::DpadLeft, new MoveLeftCommand()),
		std::pair<ButtonType, Command*>(ButtonType::DpadRight, new MoveRightCommand()),
		std::pair<ButtonType,Command*>(ButtonType::Back, new CloseWindowCommand()) };

	t_player.addComponent(new HealthComponent(10, 10));
	t_player.addComponent(new TransformComponent());
	// passing two of the same object as at this moment the commands for button press is the same for button held
	t_player.addComponent(new InputComponent(buttonPressMap, buttonPressMap));
	t_player.addComponent(new ForceComponent());
	t_player.addComponent(new ColliderCircleComponent(Utilities::PLAYER_RADIUS));
	t_player.addComponent(new ColourComponent(glm::linearRand(0, 255), glm::linearRand(0, 255), glm::linearRand(0, 255), 255));
}

void Game::createEnemy()
{
	m_entities.emplace_back();
	m_entities.back().addComponent(new TransformComponent());
	m_entities.back().addComponent(new AiComponent());
	m_entities.back().addComponent(new ColliderCircleComponent(Utilities::ENEMY_RADIUS));
}

void Game::createBullet(glm::vec2 t_position, glm::vec2 t_force)
{
}

void Game::setToWall(Entity& t_entity, glm::vec2 t_position)
{
	t_entity.removeAllComponents();
	t_entity.addComponent(new TransformComponent(t_position));
	t_entity.addComponent(new VisualComponent("assets//images//wall_4.png", m_renderer)); //TODO: change to wall texture when assets have been recieved.
	t_entity.addComponent(new ColliderAABBComponent(glm::vec2(Utilities::TILE_SIZE, Utilities::TILE_SIZE)));
}

void Game::setToFloor(Entity& t_entity, glm::vec2 t_position)
{
	t_entity.removeAllComponents();
	t_entity.addComponent(new TransformComponent(t_position));
	t_entity.addComponent(new VisualComponent("assets//images//floor_1b.png", m_renderer)); //TODO: change to floor texture when assets have been recieved.
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

void Game::closeWindow(const CloseWindow& t_event)
{
	m_isRunning = false;
}
