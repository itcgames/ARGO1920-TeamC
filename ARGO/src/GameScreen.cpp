#include "stdafx.h"
#include "GameScreen.h"


bool cleanUpEnemies(const Entity& t_entity)
{
	return !static_cast<HealthComponent*>(t_entity.getComponent(ComponentType::Health))->isAlive();
}

GameScreen::GameScreen(SDL_Renderer* t_renderer, EventManager& t_eventManager, Controller t_controllers[Utilities::NUMBER_OF_PLAYERS], ButtonCommandMap t_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS][Utilities::NUMBER_OF_PLAYERS]) :
	m_eventManager{ t_eventManager },
	m_transformSystem{ m_eventManager },
	m_projectileManager{ m_eventManager },
	m_controllers{ *t_controllers },
	m_levelManager{ t_renderer }
{
	setControllerButtonMap(t_controllerButtonMaps);
	int playerCount = 0;
	for (Entity& player : m_players)
	{
		createPlayer(player, playerCount);
		playerCount++;
	}
	m_entities.reserve(MAX_ENTITIES);
	for (int index = 0; index < 5; index++)
	{
		createEnemy();
	}
	setUpLevel();
}

GameScreen::~GameScreen()
{
	m_entities.clear();
}

void GameScreen::update(Uint16 t_deltaTime)
{
	m_levelManager.update(&m_collisionSystem);
	preRender();
	updateEntities();
	updatePlayers();
	updateProjectiles();
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


void GameScreen::createPlayer(Entity& t_player, int t_index)
{
	t_player.addComponent(new HealthComponent(10, 10));
	t_player.addComponent(new TransformComponent());
	t_player.addComponent(new InputComponent(m_controllers[t_index],
		m_controllerButtonMaps[(int)ButtonState::Pressed][t_index],
		m_controllerButtonMaps[(int)ButtonState::Held][t_index],
		m_controllerButtonMaps[(int)ButtonState::Released][t_index]));
	t_player.addComponent(new ForceComponent());
	t_player.addComponent(new ColliderCircleComponent(Utilities::PLAYER_RADIUS));
	t_player.addComponent(new ColourComponent(glm::linearRand(0, 255), glm::linearRand(0, 255), glm::linearRand(0, 255), 255));
	t_player.addComponent(new CommandComponent());
	t_player.addComponent(new TagComponent(Tag::Player));
	t_player.addComponent(new ParticleEmitterComponent(static_cast<TransformComponent*>(t_player.getComponent(ComponentType::Transform))->getPos(), true,
		Utilities::PARTICLE_DIRECTION_ANGLE_SAMPLE, Utilities::PARTICLE_OFFSET_ANGLE_SAMPLE, Utilities::PARTICLE_SPEED_SAMPLE,
		Utilities::PARTICLE_MAX_PARTICLES_SAMPLE, Utilities::PARTICLES_PER_SECOND_SAMPLE));
	t_player.addComponent(new PrimitiveComponent());
	t_player.addComponent(new FireRateComponent(Utilities::PLAYER_FIRE_DELAY));


}

void GameScreen::createEnemy()
{
	m_entities.emplace_back();
	m_entities.back().addComponent(new TransformComponent());
	m_entities.back().addComponent(new AiComponent());
	m_entities.back().addComponent(new ColliderCircleComponent(Utilities::ENEMY_RADIUS));
	m_entities.back().addComponent(new TagComponent(Tag::Enemy));
	m_entities.back().addComponent(new ForceComponent());
	m_entities.back().addComponent(new HealthComponent(Utilities::ENEMY_HP, Utilities::ENEMY_HP));
}

void GameScreen::setUpLevel()
{
	m_levelManager.setupLevel();
	//magic numbers for creating a sandbox level plz ignore.
	m_levelManager.createRoom(glm::vec2(1, 1), 12, 12);
	m_levelManager.createRoom(glm::vec2(12, 2), 3, 2);
	m_levelManager.createRoom(glm::vec2(12, 10), 3, 2);
	m_levelManager.createRoom(glm::vec2(15, 1), 5, 12);
}
 

void GameScreen::updatePlayers()
{
	for (Entity& player : m_players)
	{
		m_inputSystem.update(player);
		m_commandSystem.update(player, m_eventManager);
		m_healthSystem.update(player);
		m_transformSystem.update(player);
		m_collisionSystem.update(player);
		m_particleSystem.update(player);
	}
}

void GameScreen::updateEntities()
{
	for (Entity& entity : m_entities)
	{
		m_healthSystem.update(entity);
		m_aiSystem.update(entity);
		m_transformSystem.update(entity);
		m_collisionSystem.update(entity);
	}
} 

void GameScreen::updateProjectiles()
{
	m_projectileManager.update(&m_transformSystem);
	m_projectileManager.update(&m_collisionSystem);
}

void GameScreen::setControllerButtonMap(ButtonCommandMap t_controllerMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS][Utilities::NUMBER_OF_PLAYERS])
{
	for (int index = 0; index < Utilities::NUMBER_OF_PLAYERS; index++)
	{
		m_controllerButtonMaps[(int)ButtonState::Pressed][index] = t_controllerMaps[(int)ButtonState::Pressed][index];
		m_controllerButtonMaps[(int)ButtonState::Held][index] = t_controllerMaps[(int)ButtonState::Held][index];
		m_controllerButtonMaps[(int)ButtonState::Released][index] = t_controllerMaps[(int)ButtonState::Released][index];
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
	m_renderSystem.setFocus(focusPoint / 4.0f);
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
