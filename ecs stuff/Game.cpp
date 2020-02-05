#include "Game.h"

Game::Game()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		throw getErrorString("Error Loading SDL");
		isRunning = false;
	}
	else
	{
		// create window
		m_window = SDL_CreateWindow("Entity Component System", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, NULL);
		if (!m_window)
		{
			std::cout << getErrorString("Error Loading Window") << std::endl;
		}
		// create renderer
		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);
		if (!m_renderer)
		{
			std::cout << getErrorString("Error Loading Renderer") << std::endl;
		}
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
		isRunning = true;

		//create entities
		//m_player = new Entity(PLAYER_ID);
		//m_alien = new Entity(ALIEN_ID);
		//m_dog = new Entity(DOG_ID);
		//m_cat = new Entity(CAT_ID);

		//create an entity using make_unique incase 'new' keyword throws
		m_smartVector.emplace_back(std::make_unique<Entity>(PLAYER_ID));//0
		m_smartVector.emplace_back(std::make_unique<Entity>(ALIEN_ID));//1
		m_smartVector.emplace_back(std::make_unique<Entity>(DOG_ID));//2
		m_smartVector.emplace_back(std::make_unique<Entity>(CAT_ID));//3

		//player
		m_smartVector.at(PLAYER_ID).get()->addSmartComponent(std::make_unique<HealthComponent>());
		m_smartVector.at(PLAYER_ID).get()->addSmartComponent(std::make_unique<PositionComponent>());
		m_smartVector.at(PLAYER_ID).get()->addSmartComponent(std::make_unique<InputComponent>());

		//alien
		m_smartVector.at(ALIEN_ID).get()->addSmartComponent(std::make_unique<PositionComponent>());

		//dog
		m_smartVector.at(DOG_ID).get()->addSmartComponent(std::make_unique<PositionComponent>());

		//cat
		m_smartVector.at(CAT_ID).get()->addSmartComponent(std::make_unique<PositionComponent>());

		//add a raw ptr to systems as they only need to access/change data and take no ownership
		m_hpSystem.addEntity(m_smartVector.at(0).get()); //add player to hp system

		for (auto& ent : m_smartVector)
		{
			m_posSystem.addEntity(ent.get()); // add all entities to pos system
		}
		m_inputSystem.addEntity(m_smartVector.at(PLAYER_ID).get());//add player to input system
		m_aiSystem.addEntity(m_smartVector.at(ALIEN_ID).get());//add alien to ai system

		for (auto& ent : m_smartVector)
		{
			m_renderSystem.addEntity(ent.get()); // add all entities to pos system
		}


		////add components to player
		//m_player->addComponent(new HealthComponent(5));
		//m_player->addComponent(new PositionComponent());

		////add components to alien
		//m_alien->addComponent(new HealthComponent(10));
		//m_alien->addComponent(new PositionComponent());

		////add components to dog
		//m_dog->addComponent(new HealthComponent(3));
		//m_dog->addComponent(new PositionComponent());
		//m_dog->addComponent(new InputComponent());

		////add components to cat
		//m_cat->addComponent(new PositionComponent());

		////add entities with Health to Health System
		//m_hpSystem.addEntity(m_player);
		//m_hpSystem.addEntity(m_alien);
		//m_hpSystem.addEntity(m_dog);

		////add entities with Position to Position System
		//m_posSystem.addEntity(m_player);
		//m_posSystem.addEntity(m_alien);
		//m_posSystem.addEntity(m_dog);
		//m_posSystem.addEntity(m_cat);

		////add entities with Input to Input System
		//m_inputSystem.addEntity(m_dog);

		////add entities with Position to Render System
		//m_renderSystem.addEntity(m_player);
		//m_renderSystem.addEntity(m_alien);
		//m_renderSystem.addEntity(m_dog);
		//m_renderSystem.addEntity(m_cat);

		//add entities with AI to AI System
		//m_aiSystem.addEntity(m_alien);
	}
}

Game::~Game()
{
	cleanup();
}

void Game::run()
{
	while (isRunning)
	{
		processEvents();
		update();
		render();
	}
	cleanup();
}

void Game::processEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}

	handleInput(&event);
}

void Game::update()
{
	m_hpSystem.update();
	m_inputSystem.update();
	m_aiSystem.update();
	m_posSystem.update();
}

void Game::render()
{
	SDL_RenderClear(m_renderer);

	m_renderSystem.render(m_renderer);

	SDL_RenderPresent(m_renderer);
}

void Game::cleanup()
{
	if (m_texture)
	{
		SDL_DestroyTexture(m_texture);

	}
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	SDL_QUIT;
	SDL_Quit();
}

void Game::handleInput(SDL_Event* t_event)
{
	if (SDL_KEYDOWN == t_event->type)
	{
		if (SDLK_ESCAPE == t_event->key.keysym.sym)
		{
			isRunning = false;
		}
		if (SDLK_SPACE == t_event->key.keysym.sym)
		{
			m_smartVector.erase(m_smartVector.end() - 1);
			m_smartVector.at(m_smartVector.size() - 1) = nullptr;
		}
	}
}

std::string Game::getErrorString(std::string t_errorMsg)
{
	std::string sdlError = SDL_GetError();
	return t_errorMsg += " " + sdlError;
}
