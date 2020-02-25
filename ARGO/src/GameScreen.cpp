#include "stdafx.h"
#include "GameScreen.h"


bool cleanUpEnemies(const Entity& t_entity)
{
	return !static_cast<HealthComponent*>(t_entity.getComponent(ComponentType::Health))->isAlive();
}

GameScreen::GameScreen(SDL_Renderer* t_renderer, EventManager& t_eventManager, Controller t_controllers[Utilities::S_MAX_PLAYERS]) :
	m_eventManager{ t_eventManager },
	m_controllers{ *t_controllers },
	m_levelManager{ t_renderer, m_players, m_renderSystem },
	m_enemyManager{ t_renderer, Utilities::ENEMY_INITIAL_SPAWN_DELAY, t_eventManager, m_transformSystem, m_collisionSystem, m_healthSystem, m_aiSystem, m_renderSystem, m_levelManager },
	m_renderer{ t_renderer },
	m_transformSystem{ m_eventManager },
	m_projectileManager{ t_renderer, m_eventManager, m_renderSystem.getFocus(), m_transformSystem, m_collisionSystem },
	m_aiSystem{ m_players, m_enemyManager.getEnemies(), m_eventManager, m_levelManager },
	m_collisionSystem{ m_eventManager },
	m_playerFactory(),
	m_enemyFactory(),
	m_pickUpManager(m_eventManager, m_collisionSystem)
{
}

GameScreen::~GameScreen()
{
}

void GameScreen::update(float t_deltaTime)
{
	updateLevelManager();
	updateEntities(t_deltaTime);
	updatePlayers(t_deltaTime);
	updateProjectiles(t_deltaTime);
	m_collisionSystem.update(m_goal);
	m_collisionSystem.handleCollisions();
	m_pickUpManager.update(t_deltaTime);
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
		case SDLK_DELETE:
		{
			m_players[0].removeCompType(ComponentType::Input);
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
	m_enemyManager.render(t_renderer);
	for (Entity& player : m_players)
	{
		if (static_cast<HealthComponent*>(player.getComponent(ComponentType::Health))->isAlive())
		{
			m_renderSystem.render(t_renderer, player);
		}
	}
	m_renderSystem.render(t_renderer, m_goal);
	m_projectileManager.render(t_renderer, &m_renderSystem);
	m_pickUpManager.render(t_renderer, &m_renderSystem);
}


void GameScreen::createPlayer(Entity& t_player, int t_index, SDL_Renderer* t_renderer)
{
	bool humanControlledPlayer = true;
	if (!m_controllers[t_index].getSDLController())
	{
		humanControlledPlayer = false;
	}
	m_playerFactory.createPlayer(t_player, humanControlledPlayer, m_controllers[t_index], t_index, m_controllerButtonMaps);
}

void GameScreen::createGoal()
{
	m_goal.addComponent(new TransformComponent(Utilities::GOAL_START_POSITION));
	m_goal.addComponent(new ColliderCircleComponent(32));
	m_goal.addComponent(new TagComponent(Tag::Goal));
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
		if (static_cast<HealthComponent*>(player.getComponent(ComponentType::Health))->isAlive())
		{
			m_inputSystem.update(player);
			m_commandSystem.update(player, m_eventManager);
			m_aiSystem.update(player);
			m_healthSystem.update(player, t_deltaTime);
			m_transformSystem.update(player, t_deltaTime);
			m_collisionSystem.update(player);
			m_particleSystem.update(player, t_deltaTime);
		}
	}
}

void GameScreen::updateEntities(float t_deltaTime)
{
	m_enemyManager.update(t_deltaTime);
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
	float numberOfPoints = 0;
	for (Entity& player : m_players)
	{
		TransformComponent* transformComp = static_cast<TransformComponent*>(player.getComponent(ComponentType::Transform));
		InputComponent* inputComp = static_cast<InputComponent*>(player.getComponent(ComponentType::Input));
		HealthComponent* healthComp = static_cast<HealthComponent*>(player.getComponent(ComponentType::Health));
		if (transformComp && inputComp && healthComp->isAlive())
		{
			numberOfPoints++;
			focusPoint += transformComp->getPos();
		}
	}
	if (numberOfPoints == 0)
	{
		focusPoint = static_cast<TransformComponent*>(m_players[0].getComponent(ComponentType::Transform))->getPos();
		numberOfPoints = 1;
	}
	m_renderSystem.setFocus(focusPoint / numberOfPoints);
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
	m_playerFactory.initialise(t_renderer);
	for (Entity& player : m_players)
	{
		createPlayer(player, playerCount, t_renderer);
		playerCount++;
	}
	m_enemyManager.init();
	setUpLevel();
	m_projectileManager.init();
	createGoal();
	m_pickUpManager.init(m_renderer);
}
