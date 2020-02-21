#include "stdafx.h"
#include "Game.h"


bool cleanUpEnemies(const Entity& t_entity)
{
	return !static_cast<HealthComponent*>(t_entity.getComponent(ComponentType::Health))->isAlive();
}

/// <summary>
/// Constructor for the game class.
/// </summary>

class State;
Game::Game() :
	m_transformSystem{ m_eventManager },
	m_projectileManager{ m_eventManager, m_renderSystem.getFocus(), m_transformSystem, m_collisionSystem },
	m_levelManager(m_renderer),
	m_aiSystem(m_players, m_entities, m_eventManager)
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
		createRenderer();

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

		m_levelManager.setupLevel();

		m_levelManager.createRoom(glm::vec2(1, 1), 12, 12);
		m_levelManager.createRoom(glm::vec2(13, 2), 2, 2);
		m_levelManager.createRoom(glm::vec2(15, 1), 5, 12);
		m_levelManager.createRoom(glm::vec2(3, 13), 2, 2);
		m_levelManager.createRoom(glm::vec2(1, 15), 5, 12);
		m_levelManager.createRoom(glm::vec2(6, 23), 2, 2);
		m_levelManager.createRoom(glm::vec2(8, 15), 20, 18);
		m_levelManager.createRoom(glm::vec2(17, 13), 2, 2);
		m_levelManager.createRoom(glm::vec2(17, 13), 2, 2);
		m_levelManager.createRoom(glm::vec2(1, 29), 5, 10);
		m_levelManager.createRoom(glm::vec2(6, 35), 8, 4);
		m_levelManager.createRoom(glm::vec2(10, 33), 2, 2);
		m_levelManager.createRoom(glm::vec2(22, 1), 20, 5);
		m_levelManager.createRoom(glm::vec2(24, 6), 2, 9);
		m_levelManager.createRoom(glm::vec2(30, 8), 6, 14);
		m_levelManager.createRoom(glm::vec2(32, 6), 2, 2);
		m_levelManager.createRoom(glm::vec2(28, 18), 2, 2);
		m_levelManager.createRoom(glm::vec2(24, 33), 2, 2);
		m_levelManager.createRoom(glm::vec2(16, 35), 14, 4);
		m_levelManager.createRoom(glm::vec2(28, 29), 2, 2);
		m_levelManager.createRoom(glm::vec2(30, 24), 4, 9);
		m_levelManager.createRoom(glm::vec2(34, 24), 5, 15);
		m_levelManager.createRoom(glm::vec2(30, 36), 4, 2);
		m_levelManager.createRoom(glm::vec2(42, 2), 2, 2);
		m_levelManager.createRoom(glm::vec2(44, 1), 14, 8);
		m_levelManager.createRoom(glm::vec2(36, 14), 2, 2);
		m_levelManager.createRoom(glm::vec2(38, 12), 10, 9);
		m_levelManager.createRoom(glm::vec2(50, 12), 8, 20);
		m_levelManager.createRoom(glm::vec2(41, 23), 9, 6);
		m_levelManager.createRoom(glm::vec2(39, 25), 2, 2);
		m_levelManager.createRoom(glm::vec2(48, 14), 2, 2);
		m_levelManager.createRoom(glm::vec2(54, 32), 2, 2);
		m_levelManager.createRoom(glm::vec2(41, 34), 17, 4);
		m_levelManager.createRoom(glm::vec2(41, 31), 7, 3);

		setupIgnoredEvents();
	}
	catch (std::string error)
	{
		// output error if any part above doesnt load correctly
		std::cout << error << std::endl;
		// game doesnt run
		m_isRunning = false;
	}
	m_projectileManager.init();
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
	Uint32 timePerFrame = 1000 / 60;
	Uint32 lastTick = SDL_GetTicks();
	Uint32 nextFrame = SDL_GetTicks() + timePerFrame;
	Uint32 currentTick = 0; 
	Uint32 timeSinceLastTick = 0;
	while (m_isRunning)
	{
		processEvent(); 
		while (SDL_GetTicks() < nextFrame)
		{
			currentTick = SDL_GetTicks();
			timeSinceLastTick = currentTick - lastTick;
			if (timeSinceLastTick > 0)
			{
				processEvent();
				float t_dt = (float)timeSinceLastTick / (float)timePerFrame;
				lastTick = currentTick;
				update(t_dt);
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
				m_audioMgr->SetMasterVolume(m_audioMgr->GetMasterVolume() + Utilities::AUDIO_VOLUME_STEP);
			}
			if (SDLK_DOWN == event.key.keysym.sym)
			{
				m_audioMgr->SetMasterVolume(m_audioMgr->GetMasterVolume() - Utilities::AUDIO_VOLUME_STEP);
			}
			//sfx volume
			if (SDLK_KP_7 == event.key.keysym.sym)
			{
				m_audioMgr->SetSfxVolume(m_audioMgr->GetSfxVolume() + Utilities::AUDIO_VOLUME_STEP);
			}
			if (SDLK_KP_4 == event.key.keysym.sym)
			{
				m_audioMgr->SetSfxVolume(m_audioMgr->GetSfxVolume() - Utilities::AUDIO_VOLUME_STEP);
			}
			//music volume
			if (SDLK_KP_9 == event.key.keysym.sym)
			{
				m_audioMgr->SetMusicVolume(m_audioMgr->GetMusicVolume() + Utilities::AUDIO_VOLUME_STEP);
			}
			if (SDLK_KP_6 == event.key.keysym.sym)
			{
				m_audioMgr->SetMusicVolume(m_audioMgr->GetMusicVolume() - Utilities::AUDIO_VOLUME_STEP);
			}
			break;
		default:
			break;
		}
	}
}

void Game::update(float t_dt)
{
	m_levelManager.checkWallDamage();
	m_levelManager.update(&m_collisionSystem);
	for (auto& entity : m_entities)
	{
		m_hpSystem.update(entity);
		m_aiSystem.update(entity);
		m_transformSystem.update(entity, t_dt);
		m_collisionSystem.update(entity);
	}
	for (auto& player : m_players)
	{
		m_hpSystem.update(player);
		m_inputSystem.update(player);
		m_commandSystem.update(player, m_eventManager);
		m_aiSystem.update(player);
		m_transformSystem.update(player, t_dt);
		m_collisionSystem.update(player);
		m_particleSystem.update(player, t_dt);
	}
	m_projectileManager.tick(t_dt);
	m_projectileManager.update(t_dt);
	m_collisionSystem.handleCollisions();

	removeDeadEnemies();
}

void Game::render()
{
	preRender();
	m_levelManager.render(m_renderer, &m_renderSystem);
	for (auto& entity : m_entities)
	{
		m_renderSystem.render(m_renderer, entity);
	}
	for (auto& player : m_players)
	{
		m_renderSystem.render(m_renderer, player);
	}
	m_projectileManager.render(m_renderer, &m_renderSystem);
	SDL_RenderPresent(m_renderer);
}

void Game::preRender()
{
	//setting the focus point for the camera.
	glm::vec2 focusPoint = glm::vec2(0, 0);
	for (auto& player : m_players)
	{
		TransformComponent* transformComp = static_cast<TransformComponent*>(player.getComponent(ComponentType::Transform));
		if (transformComp)
		{
			focusPoint += transformComp->getPos();
		}
		else
		{
			throw std::invalid_argument("Player missing Transform Component!");
		}
	}
	m_renderSystem.setFocus(focusPoint / (float)Utilities::S_MAX_PLAYERS);

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

void Game::createPlayer(Entity& t_player)
{
	std::map<ButtonType, Command*> buttonPressMap = {
		std::pair<ButtonType, Command*>(ButtonType::DpadUp, new MoveUpCommand()),
		std::pair<ButtonType, Command*>(ButtonType::DpadDown, new MoveDownCommand()),
		std::pair<ButtonType, Command*>(ButtonType::DpadLeft, new MoveLeftCommand()),
		std::pair<ButtonType, Command*>(ButtonType::DpadRight, new MoveRightCommand()),
		std::pair<ButtonType, Command*>(ButtonType::RightTrigger, new FireBulletCommand()),
		std::pair<ButtonType,Command*>(ButtonType::Back, new CloseWindowCommand()) };

	InputComponent* impComp = new InputComponent(buttonPressMap, buttonPressMap);
	if (impComp->getController().getSDLController())
	{
		// passing two of the same object as at this moment the commands for button press is the same for button held
		t_player.addComponent(impComp);
	}
	else
	{
		t_player.addComponent(new AiComponent(AITypes::ePlayerBot, AIStates::eWander, 0, 0));
	}

	t_player.addComponent(new CommandComponent());
	t_player.addComponent(new HealthComponent(10, 10));
	t_player.addComponent(new TransformComponent());
	t_player.addComponent(new ForceComponent());
	t_player.addComponent(new ColliderCircleComponent(Utilities::PLAYER_RADIUS));
	t_player.addComponent(new ColourComponent(glm::linearRand(0, 255), glm::linearRand(0, 255), glm::linearRand(0, 255), 255));
	t_player.addComponent(new ParticleEmitterComponent(static_cast<TransformComponent*>(t_player.getComponent(ComponentType::Transform))->getPos(), true,
		Utilities::PARTICLE_DIRECTION_ANGLE_SAMPLE, Utilities::PARTICLE_OFFSET_ANGLE_SAMPLE, Utilities::PARTICLE_SPEED_SAMPLE,
		Utilities::PARTICLE_MAX_PARTICLES_SAMPLE, Utilities::PARTICLES_PER_SECOND_SAMPLE));
	t_player.addComponent(new PrimitiveComponent());
	t_player.addComponent(new TagComponent(Tag::Player));
	t_player.addComponent(new FireRateComponent(Utilities::PLAYER_FIRE_DELAY));
}

void Game::createEnemy()
{
	m_entities.emplace_back();
	m_entities.back().addComponent(new TransformComponent());
	m_entities.back().addComponent(new ForceComponent());
	m_entities.back().addComponent(new AiComponent(AITypes::eMelee, AIStates::eWander, 15.0f, 1.0f));
	m_entities.back().addComponent(new ColliderCircleComponent(Utilities::ENEMY_RADIUS));
	m_entities.back().addComponent(new TagComponent(Tag::Enemy));
	m_entities.back().addComponent(new HealthComponent(Utilities::ENEMY_HP, Utilities::ENEMY_HP));
}

void Game::removeDeadEnemies()
{
	std::vector<Entity>::iterator iter = std::remove_if(m_entities.begin(), m_entities.end(), cleanUpEnemies);
	while (iter != m_entities.end())
	{
		iter->nullAllComponents();
		iter = m_entities.erase(iter);
	}
}

void Game::closeWindow(const CloseWindow& t_event)
{
	m_isRunning = false;
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