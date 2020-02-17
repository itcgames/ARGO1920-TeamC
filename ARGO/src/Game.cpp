#include "stdafx.h"
#include "Game.h"

/// <summary>
/// Constructor for the game class.
/// </summary>

class State;
Game::Game() :
	m_transformSystem{ m_eventManager },
	m_projectileManager{ m_eventManager },
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
		m_audioMgr->PlayMusic("looping\\Ove - Earth Is All We Have.ogg");

		// Game is running
		m_isRunning = true;

		m_eventManager.subscribeToEvent<CloseWindow>(std::bind(&Game::closeWindow, this, std::placeholders::_1));
		m_eventManager.subscribeToEvent<createBulletEvent>(std::bind(&Game::playerFireSound, this, std::placeholders::_1));
		m_eventManager.subscribeToEvent<createAltBulletEvent>(std::bind(&Game::playerAltFireSound, this, std::placeholders::_1));

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

		m_textTest1.addComponent(new TransformComponent());
		m_textTest1.addComponent(new TextComponent("comic.ttf", m_renderer, true, std::string("Static Text")));
		m_textTest2.addComponent(new TransformComponent());
		m_textTest2.addComponent(new TextComponent("pt-sans.ttf", m_renderer, Utilities::LARGE_FONT, false, "Not Static Text", 255, 255, 0, 123));

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
	m_entities.clear();

	AssetManager::Release();
	m_assetMgr = NULL;

	AudioManager::Release();
	m_audioMgr = NULL;

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
				//add one enemy
				createEnemy();
			}
			std::cout << m_entities.size() << std::endl;
		}
		if (SDLK_q == event.key.keysym.sym)
		{
			m_audioMgr->PlaySfx("airhorn.wav");
		}
		//master volume
		if (SDLK_UP == event.key.keysym.sym)
		{
			m_audioMgr->SetMasterVolume(m_audioMgr->GetMasterVolume() + 10);
		}
		if (SDLK_DOWN == event.key.keysym.sym)
		{
			m_audioMgr->SetMasterVolume(m_audioMgr->GetMasterVolume() - 10);
		}
		//sfx volume
		if (SDLK_KP_7 == event.key.keysym.sym)
		{
			m_audioMgr->SetSfxVolume(m_audioMgr->GetSfxVolume() + 10);
		}
		if (SDLK_KP_4 == event.key.keysym.sym)
		{
			m_audioMgr->SetSfxVolume(m_audioMgr->GetSfxVolume() - 10);
		}
		//music volume
		if (SDLK_KP_9 == event.key.keysym.sym)
		{
			m_audioMgr->SetMusicVolume(m_audioMgr->GetMusicVolume() + 10);
		}
		if (SDLK_KP_6 == event.key.keysym.sym)
		{
			m_audioMgr->SetMusicVolume(m_audioMgr->GetMusicVolume() - 10);
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
			m_inputSystem.update(player);
			m_commandSystem.update(player, m_eventManager);
			m_hpSystem.update(player);
			m_transformSystem.update(player);
			m_collisionSystem.update(player);
		}
		if (t_canRender)
		{
			m_renderSystem.render(m_renderer, player);
		}
	}

	if (t_canRender)
	{
		m_renderSystem.render(m_renderer, m_textTest1);
		m_renderSystem.render(m_renderer, m_textTest2);
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
		TransformComponent* transformComp = static_cast<TransformComponent*>(player.getAllComps().at(COMPONENT_ID::TRANSFORM_ID));
		if (transformComp)
		{
			focusPoint += transformComp->getPos();
		}
		else
		{
			throw std::invalid_argument("Player missing Transform Component!");
		}
	}
	m_renderSystem.setFocus(focusPoint / 4.0f);

	SDL_RenderClear(m_renderer);
}

/// <summary>
/// Cleans up after running by deleting stuff
/// </summary>
void Game::cleanup()
{
	IMG_Quit();
	TTF_Quit();
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	m_renderer = NULL;
	SDL_Quit();
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
		std::pair<ButtonType, Command*>(ButtonType::RightTrigger, new FireBulletCommand()),
		std::pair<ButtonType, Command*>(ButtonType::LeftTrigger, new FireAltBulletCommand()),
		std::pair<ButtonType,Command*>(ButtonType::Back, new CloseWindowCommand()) };

	t_player.addComponent(new CommandComponent());
	t_player.addComponent(new HealthComponent(10, 10));
	t_player.addComponent(new TransformComponent());
	// passing two of the same object as at this moment the commands for button press is the same for button held
	t_player.addComponent(new InputComponent(buttonPressMap, buttonPressMap));
	t_player.addComponent(new ForceComponent());
	t_player.addComponent(new ColliderCircleComponent(Utilities::PLAYER_RADIUS));
	t_player.addComponent(new ColourComponent(glm::linearRand(0, 255), glm::linearRand(0, 255), glm::linearRand(0, 255), 255));
	t_player.addComponent(new TagComponent(Tag::Player));
}

void Game::createEnemy()
{
	m_entities.emplace_back();
	m_entities.back().addComponent(new TransformComponent());
	m_entities.back().addComponent(new AiComponent());
	m_entities.back().addComponent(new ColliderCircleComponent(Utilities::ENEMY_RADIUS));
	m_entities.back().addComponent(new TagComponent(Tag::Enemy));
}

void Game::setToWall(Entity& t_entity, glm::vec2 t_position)
{
	t_entity.removeAllComponents();
	t_entity.addComponent(new TransformComponent(t_position));
	t_entity.addComponent(new VisualComponent("wall_4.png", m_renderer)); //TODO: change to wall texture when assets have been recieved.
	t_entity.addComponent(new ColliderAABBComponent(glm::vec2(Utilities::TILE_SIZE, Utilities::TILE_SIZE)));
	t_entity.addComponent(new TagComponent(Tag::Wall));
}

void Game::setToFloor(Entity& t_entity, glm::vec2 t_position)
{
	t_entity.removeAllComponents();
	t_entity.addComponent(new TransformComponent(t_position));
	t_entity.addComponent(new VisualComponent("floor_1b.png", m_renderer)); //TODO: change to floor texture when assets have been recieved.
}

void Game::playerFireSound(const createBulletEvent& t_event)
{
	m_audioMgr->PlayPlayerFireSfx(Utilities::GUN_FIRE_PATH + "launcher.wav", static_cast<TransformComponent*>(t_event.entity.getComponent(ComponentType::Transform))->getPos(), m_renderSystem.getFocus());
}

void Game::playerAltFireSound(const createAltBulletEvent& t_event)
{
	m_audioMgr->PlayPlayerFireSfx(Utilities::GUN_FIRE_PATH + "ak.wav", static_cast<TransformComponent*>(t_event.entity.getComponent(ComponentType::Transform))->getPos(), m_renderSystem.getFocus());
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