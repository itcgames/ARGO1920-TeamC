#include "stdafx.h"
#include "GameScreen.h"


bool cleanUpEnemies(const Entity& t_entity)
{
	return !static_cast<HealthComponent*>(t_entity.getComponent(ComponentType::Health))->isAlive();
}

GameScreen::GameScreen(SDL_Renderer* t_renderer, EventManager& t_eventManager, Controller t_controllers[Utilities::S_MAX_PLAYERS]) :
	m_eventManager{ t_eventManager },
	m_controllers{ *t_controllers },
	m_levelManager{ t_renderer },
	m_transformSystem{ m_eventManager },
	m_projectileManager{ m_eventManager, m_renderSystem.getFocus(), m_transformSystem, m_collisionSystem },
	m_aiSystem{ m_players, m_entities, m_eventManager }
{
}

GameScreen::~GameScreen()
{
	m_entities.clear();
}

void GameScreen::update(float t_deltaTime)
{
	updateLevelManager();
	updateEntities(t_deltaTime);
	updatePlayers(t_deltaTime);
	updateProjectiles(t_deltaTime);
	m_collisionSystem.handleCollisions();
	removeDeadEnemies();
}

void GameScreen::processEvents(SDL_Event* t_event)
{
	switch (t_event->type)
	{
	case SDL_KEYDOWN:
	{
		switch (t_event->key.keysym.sym)
		{
		case SDLK_HOME:
		{
			m_eventManager.emitEvent<ChangeScreen>(ChangeScreen{ MenuStates::MainMenu });
			break;
		}
		case SDLK_BACKSPACE:
		{
			//delete all entities
			if (!m_entities.empty())
			{
				m_entities.erase(m_entities.begin(), m_entities.end());
			}
			std::cout << m_entities.size() << std::endl;
			break;
		}
		case SDLK_DELETE:
		{
			//delete all entities
			m_players[0].removeCompType(ComponentType::Input);
			break;
		}
		case SDLK_RETURN:
		{
			//check if we can add 100 entities, if more than 100, set to 100, if less than 0 set to 0
			int availableSpace = glm::clamp(int(MAX_ENTITIES - m_entities.size()), 0, 100);
			for (int index = 0; index < availableSpace; index++)
			{
				createEnemy();
			}
			std::cout << m_entities.size() << std::endl;
			break;
		}
		case SDLK_1:
		{
			//create one enemy if space available in the vector
			if (m_entities.size() < MAX_ENTITIES)
			{
				createEnemy();
			}
			std::cout << m_entities.size() << std::endl;
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


void GameScreen::render(SDL_Renderer* t_renderer)
{
	preRender();
	m_levelManager.render(t_renderer, &m_renderSystem);
	for (Entity& entity : m_entities)
	{
		m_renderSystem.render(t_renderer, entity);
	}
	for (Entity& player : m_players)
	{
		m_renderSystem.render(t_renderer, player);
	}
	m_projectileManager.render(t_renderer, &m_renderSystem);
}


void GameScreen::createPlayer(Entity& t_player, int t_index, SDL_Renderer* t_renderer)
{
	t_player.addComponent(new HealthComponent(10, 10));
	t_player.addComponent(new TransformComponent());

	t_player.addComponent(new ForceComponent());
	t_player.addComponent(new ColliderCircleComponent(Utilities::PLAYER_RADIUS));
	t_player.addComponent(new VisualComponent("player.png", t_renderer, static_cast<Uint8>(glm::linearRand(0, 255)), static_cast<Uint8>(glm::linearRand(0, 255)), static_cast<Uint8>(glm::linearRand(0, 255))));
	t_player.addComponent(new CommandComponent());
	t_player.addComponent(new TagComponent(Tag::Player));
	t_player.addComponent(new ParticleEmitterComponent(static_cast<TransformComponent*>(t_player.getComponent(ComponentType::Transform))->getPos(), true,
		Utilities::PARTICLE_DIRECTION_ANGLE_SAMPLE, Utilities::PARTICLE_OFFSET_ANGLE_SAMPLE, Utilities::PARTICLE_SPEED_SAMPLE,
		Utilities::PARTICLE_MAX_PARTICLES_SAMPLE, Utilities::PARTICLES_PER_SECOND_SAMPLE));
	t_player.addComponent(new FireRateComponent(Utilities::PLAYER_FIRE_DELAY));
	if (m_controllers[t_index].getSDLController())
	{
		t_player.addComponent(new InputComponent(m_controllers[t_index],
			m_controllerButtonMaps[static_cast<int>(ButtonState::Pressed)][t_index],
			m_controllerButtonMaps[static_cast<int>(ButtonState::Held)][t_index],
			m_controllerButtonMaps[static_cast<int>(ButtonState::Released)][t_index]));
	}
	else
	{
		t_player.addComponent(new AiComponent(AITypes::ePlayerBot, AIStates::eWander, 0, 0));
	}
}

void GameScreen::createEnemy()
{
	m_entities.emplace_back();
	m_entities.back().addComponent(new TransformComponent());
	m_entities.back().addComponent(new AiComponent(AITypes::eMelee, AIStates::eWander, 15.0f, 1.0f));
	m_entities.back().addComponent(new ColliderCircleComponent(Utilities::ENEMY_RADIUS));
	m_entities.back().addComponent(new TagComponent(Tag::Enemy));
	m_entities.back().addComponent(new ForceComponent());
	m_entities.back().addComponent(new HealthComponent(Utilities::ENEMY_HP, Utilities::ENEMY_HP));
}

void GameScreen::setUpLevel()
{
	m_levelManager.setupLevel();
	//Creates a room at a given position with a given width and height
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
}


void GameScreen::updatePlayers(float t_deltaTime)
{
	for (Entity& player : m_players)
	{
		m_inputSystem.update(player);
		m_commandSystem.update(player, m_eventManager);
		m_aiSystem.update(player);
		m_healthSystem.update(player);
		m_transformSystem.update(player, t_deltaTime);
		m_collisionSystem.update(player);
		m_particleSystem.update(player, t_deltaTime);
	}
}

void GameScreen::updateEntities(float t_deltaTime)
{
	for (Entity& entity : m_entities)
	{
		m_healthSystem.update(entity);
		m_aiSystem.update(entity);
		m_transformSystem.update(entity, t_deltaTime);
		m_collisionSystem.update(entity);
	}
}

void GameScreen::updateProjectiles(float t_deltaTime)
{
	m_projectileManager.update(t_deltaTime);
}

void GameScreen::updateLevelManager()
{
	m_levelManager.checkWallDamage();
	m_levelManager.update(&m_collisionSystem);
}

void GameScreen::setControllerButtonMap(ButtonCommandMap t_controllerMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS][Utilities::S_MAX_PLAYERS])
{
	for (int index = 0; index < Utilities::S_MAX_PLAYERS; index++)
	{
		m_controllerButtonMaps[static_cast<int>(ButtonState::Pressed)][index] = t_controllerMaps[static_cast<int>(ButtonState::Pressed)][index];
		m_controllerButtonMaps[static_cast<int>(ButtonState::Held)][index] = t_controllerMaps[static_cast<int>(ButtonState::Held)][index];
		m_controllerButtonMaps[static_cast<int>(ButtonState::Released)][index] = t_controllerMaps[static_cast<int>(ButtonState::Released)][index];
	}
}

void GameScreen::preRender()
{
	// Setting the focus point for the camera.
	glm::vec2 focusPoint = glm::vec2(0.0f, 0.0f);
	for (Entity& player : m_players)
	{
		TransformComponent* transformComp = static_cast<TransformComponent*>(player.getAllComps().at(COMPONENT_ID::TRANSFORM_ID));
		if (transformComp)
		{
			focusPoint += transformComp->getPos();
		}
		else
		{
			throw std::invalid_argument("Player Missing Transform Component!");
		}
	}
	m_renderSystem.setFocus(focusPoint / (float)Utilities::S_MAX_PLAYERS);
}

void GameScreen::reset(SDL_Renderer* t_renderer, Controller t_controller[Utilities::S_MAX_PLAYERS])
{
	for (int index = 0; index < Utilities::S_MAX_PLAYERS; index++)
	{
		m_controllers[index] = t_controller[index];
	}
	int playerCount = 0;
	for (Entity& player : m_players)
	{
		player.removeAllComponents();
		createPlayer(player, playerCount, t_renderer);
		playerCount++;
	}
	for (Entity& entity : m_entities)
	{
		entity.removeAllComponents();
	}
	m_entities.clear();

	for (int index = 0; index < 5; index++)
	{
		createEnemy();
	}
	setUpLevel();
}

void GameScreen::initialise(SDL_Renderer* t_renderer, ButtonCommandMap t_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS][Utilities::S_MAX_PLAYERS], Controller t_controller[Utilities::S_MAX_PLAYERS])
{
	setControllerButtonMap(t_controllerButtonMaps);
	for (int index = 0; index < Utilities::S_MAX_PLAYERS; index++)
	{
		m_controllers[index] = t_controller[index];
	}
	int playerCount = 0;
	for (Entity& player : m_players)
	{
		createPlayer(player, playerCount, t_renderer);
		playerCount++;
	}
	m_entities.reserve(MAX_ENTITIES);
	for (int index = 0; index < 5; index++)
	{
		createEnemy();
	}
	setUpLevel();
	m_projectileManager.init();
}

void GameScreen::removeDeadEnemies()
{
	std::vector<Entity>::iterator iter = std::remove_if(m_entities.begin(), m_entities.end(), cleanUpEnemies);
	while (iter != m_entities.end())
	{
		iter->nullAllComponents();
		iter = m_entities.erase(iter);
	}
}
