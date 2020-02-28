#include "stdafx.h"
#include "GameScreen.h"


bool cleanUpEnemies(const Entity& t_entity)
{
	return !static_cast<HealthComponent*>(t_entity.getComponent(ComponentType::Health))->isAlive();
}

GameScreen::GameScreen(SDL_Renderer* t_renderer, EventManager& t_eventManager, Controller t_controllers[Utilities::S_MAX_PLAYERS], CommandSystem& t_commandSystem, InputSystem& t_input, RenderSystem& t_renderSystem, OnlineGameHandler& t_onlineHandler) :
	m_eventManager{ t_eventManager },
	m_controllers{ *t_controllers },
	m_levelManager{ t_renderer },
	m_renderer{ t_renderer },
	m_transformSystem{ m_eventManager },
	m_projectileManager{ t_renderer, m_eventManager, m_renderSystem.getFocus(), m_transformSystem, m_collisionSystem },
	m_aiSystem{ m_players, m_entities, m_eventManager },
	m_collisionSystem{ m_eventManager },
	m_commandSystem{ t_commandSystem },
	m_inputSystem{ t_input },
	m_renderSystem{ t_renderSystem },
	m_onlineHandler{ t_onlineHandler }
{
}

GameScreen::~GameScreen()
{
	m_entities.clear();
}

void GameScreen::update(float t_deltaTime)
{
	switch (Utilities::OnlineData::S_ONLINE_STATUS)
	{
	case Utilities::OnlineState::Client:
	{
		updateLevelManager();
		updatePlayers(t_deltaTime);
		updateProjectiles(t_deltaTime);
		m_collisionSystem.update(m_goal);
		m_collisionSystem.handleCollisions();
		//if (static_cast<HealthComponent*>(m_players[0].getComponent(ComponentType::Health))->isAlive())
		{
			sendClientData();
		}
		processReceivedHostData();


		break;
	}
	case Utilities::OnlineState::Host:
		sendHostData();
		processReceivedClientData();
	case Utilities::OnlineState::Local:
		updateLevelManager();
		updateEntities(t_deltaTime);
		updatePlayers(t_deltaTime);
		updateProjectiles(t_deltaTime);
		m_collisionSystem.update(m_goal);
		m_collisionSystem.handleCollisions();
		removeDeadEnemies();
		break;
	default:
		break;
	}
}

void GameScreen::processEvents(SDL_Event* t_event)
{
	switch (t_event->type)
	{
	case SDL_KEYDOWN:
	{
		switch (t_event->key.keysym.sym)
		{
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
		if (static_cast<HealthComponent*>(player.getComponent(ComponentType::Health))->isAlive())
		{
			m_renderSystem.render(t_renderer, player);
		}
	}
	m_renderSystem.render(t_renderer, m_goal);
	m_projectileManager.render(t_renderer, &m_renderSystem);
}


void GameScreen::createPlayer(Entity& t_player, int t_index, SDL_Renderer* t_renderer, bool t_isOnline)
{
	t_player.addComponent(new HealthComponent(10, 10, Utilities::PLAYER_INVINCIBILITY_FRAMES));
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
	else if (Utilities::OnlineData::S_ONLINE_STATUS != Utilities::OnlineState::Client)
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
	m_entities.back().addComponent(new VisualComponent("EnemyFast.png", m_renderer));
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
		createPlayer(player, playerCount, t_renderer, false);
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

void GameScreen::initialise(SDL_Renderer* t_renderer, ButtonCommandMap t_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS][Utilities::S_MAX_PLAYERS], Controller t_controller[Utilities::S_MAX_PLAYERS], bool t_isOnline)
{
	//m_isOnline = t_isOnline;
	setControllerButtonMap(t_controllerButtonMaps);
	for (int index = 0; index < Utilities::S_MAX_PLAYERS; index++)
	{
		m_controllers[index] = t_controller[index];
	}
	int playerCount = 0;
	for (Entity& player : m_players)
	{
		createPlayer(player, playerCount, t_renderer, t_isOnline);
		playerCount++;
	}
	m_entities.reserve(MAX_ENTITIES);
	for (int index = 0; index < 5; index++)
	{
		createEnemy();
	}
	setUpLevel();
	m_projectileManager.init();
	createGoal();
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

void GameScreen::processReceivedClientData()
{
	if (!m_onlineHandler.getGameData().empty())
	{
		std::string& gameData = m_onlineHandler.getGameData();
		std::vector<std::string> tokens;
		std::size_t start = 0, end = 0;
		while ((end = gameData.find(",", start)) != std::string::npos) {
			tokens.push_back(gameData.substr(start, end - start));
			start = end + 1;
		}
		tokens.push_back(gameData.substr(start));

		std::vector<float> posVec;

		for (int i = 0; i < tokens.size(); i++)
		{
			std::stringstream ss(tokens.at(i));
			float pos;
			ss >> pos;
			posVec.push_back(pos);
		}

		//for (int i = 0; i < Utilities::S_MAX_PLAYERS; i++)
		{
			static_cast<TransformComponent*>(m_players[3].getComponent(ComponentType::Transform))->setPos(posVec[0], posVec[1]);
		}
	}
}

void GameScreen::processReceivedHostData()
{
	if (!m_onlineHandler.getGameData().empty())
	{
		std::string& gameData = m_onlineHandler.getGameData();
		std::vector<std::string> entityData;
		std::size_t start = gameData.find(":", 0) + 1;
		std::size_t end = start;
		int dataIndex = 0;

		//#############################################################################
		//Entities
		{
			while ((end = gameData.find(",", start)) != std::string::npos)
			{
				entityData.push_back(gameData.substr(start, end - start));
				start = end + 1;
				if (gameData.at(start) == '!')
				{
					break;
				}
			}

			for (int i = 0; i < m_entities.size(); i++)
			{
				TagComponent* tagComp = static_cast<TagComponent*>(m_entities.at(i).getComponent(ComponentType::Tag));
				HealthComponent* hpComp = static_cast<HealthComponent*>(m_entities.at(i).getComponent(ComponentType::Health));
				TransformComponent* posComp = static_cast<TransformComponent*>(m_entities.at(i).getComponent(ComponentType::Transform));

				if (dataIndex >= entityData.size() || entityData.at(0) == "")
				{
					break;
				}
				int data;
				std::stringstream ss(entityData.at(dataIndex));
				ss >> data;
				tagComp->setTag(data);
				ss.clear();
				ss.str(entityData.at(++dataIndex));
				ss >> data;
				posComp->setX(data);
				ss.clear();
				ss.str(entityData.at(++dataIndex));
				ss >> data;
				posComp->setY(data);
				ss.clear();
				ss.str(entityData.at(++dataIndex));
				ss >> data;
				hpComp->setHealth(data);
				dataIndex++;
			}
		}


		//#############################################################################
		//Players
		{
			entityData.clear();
			start = gameData.find(":", start) + 1;
			end = start;

			while ((end = gameData.find(",", start)) != std::string::npos)
			{
				entityData.push_back(gameData.substr(start, end - start));
				start = end + 1;
				if (gameData.at(start) == '!' || entityData.at(0) == "")
				{
					break;
				}
			}

			dataIndex = 0;
			for (int i = 1; i < 3; i++)
			{
				HealthComponent* hpComp = static_cast<HealthComponent*>(m_players[i].getComponent(ComponentType::Health));
				TransformComponent* posComp = static_cast<TransformComponent*>(m_players[i].getComponent(ComponentType::Transform));

				if (dataIndex >= entityData.size())
				{
					break;
				}
				int data;
				std::stringstream ss(entityData.at(dataIndex));
				ss.str(entityData.at(++dataIndex));
				ss >> data;
				posComp->setX(data);
				ss.clear();
				ss.str(entityData.at(++dataIndex));
				ss >> data;
				posComp->setY(data);
				ss.clear();
				ss >> data;
				hpComp->setHealth(data);

				dataIndex++;
			}
		}


		//#############################################################################
		//Tiles
		{
			entityData.clear();
			start = gameData.find(":", start) + 1;
			end = start;

			while ((end = gameData.find(",", start)) != std::string::npos)
			{
				entityData.push_back(gameData.substr(start, end - start));
				start = end + 1;
				if (start < gameData.size() && gameData.at(start) == '!')
				{
					break;
				}
			}

			std::vector<Entity>& tiles = m_levelManager.getTiles();
			dataIndex = 0;
			for (int i = 0; i < tiles.size(); i++)
			{
				TileComponent* tileComp = static_cast<TileComponent*>(tiles.at(i).getComponent(ComponentType::Tile));

				if (dataIndex >= entityData.size() || entityData.at(0) == "")
				{
					break;
				}
				int data;
				std::stringstream ss(entityData.at(dataIndex));
				ss >> data;
				if (static_cast<int>(tileComp->getTileType()) != data)
				{
					if (data == 0)
					{
						m_levelManager.setToWall(tiles.at(i));
					}
					else if (data == 1)
					{
						m_levelManager.setToFloor(tiles.at(i));
					}
					tileComp->setTileType(static_cast<TileType>(data));
				}
				dataIndex++;
			}
		}
	}
}

void GameScreen::sendHostData()
{
	std::string entityData;
	std::vector<Entity>& levelTiles = m_levelManager.getTiles();

	//#############################################################################
	//Entities
	entityData.append(Utilities::ONLINE_ENTITY);
	for (int i = 0; i < m_entities.size(); i++)
	{
		entityData += std::to_string(static_cast<int>(static_cast<TagComponent*>(m_entities.at(i).getComponent(ComponentType::Tag))->getTag()));
		entityData += Utilities::ONLINE_ENTITY_DATA_SEPARATOR;
		glm::ivec2 pos = static_cast<TransformComponent*>(m_entities.at(i).getComponent(ComponentType::Transform))->getPos();
		entityData += std::to_string(pos.x);
		entityData += Utilities::ONLINE_ENTITY_DATA_SEPARATOR;
		entityData += std::to_string(pos.y);
		entityData += Utilities::ONLINE_ENTITY_DATA_SEPARATOR;
		entityData += std::to_string(static_cast<int>(static_cast<HealthComponent*>(m_entities.at(i).getComponent(ComponentType::Health))->getHealth()));
		entityData += Utilities::ONLINE_ENTITY_SEPARATOR;
	}
	if (entityData[entityData.size() - 1] != ',')
	{
		entityData += Utilities::ONLINE_ENTITY_SEPARATOR;
	}
	entityData += Utilities::ONLINE_ENTITY_TYPE_SEPARATOR;

	//#############################################################################
	//Players
	std::string playerData = "";
	playerData += Utilities::ONLINE_PLAYER;
	//change to amount of host's local players once logic is in for that
	for (int i = 0; i < 3; i++)
	{
		glm::ivec2 pos = static_cast<TransformComponent*>(m_players[i].getComponent(ComponentType::Transform))->getPos();
		playerData += std::to_string(pos.x);
		playerData += Utilities::ONLINE_ENTITY_DATA_SEPARATOR;
		playerData += std::to_string(pos.y);
		playerData += Utilities::ONLINE_ENTITY_DATA_SEPARATOR;
		playerData += std::to_string(static_cast<int>(static_cast<HealthComponent*>(m_players[i].getComponent(ComponentType::Health))->getHealth()));
		playerData += Utilities::ONLINE_ENTITY_SEPARATOR;
	}
	if (playerData[playerData.size() - 1] != ',')
	{
		playerData += Utilities::ONLINE_ENTITY_SEPARATOR;
	}
	playerData += Utilities::ONLINE_ENTITY_TYPE_SEPARATOR;

	//#############################################################################
	//Tiles
	std::string tileData = "";
	tileData += Utilities::ONLINE_TILE;
	for (int i = 0; i < levelTiles.size(); i++)
	{
		tileData += std::to_string(static_cast<int>(static_cast<TileComponent*>(levelTiles.at(i).getComponent(ComponentType::Tile))->getTileType()));
		tileData += Utilities::ONLINE_ENTITY_SEPARATOR;
	}
	if (tileData[tileData.size() - 1] != ',')
	{
		tileData += Utilities::ONLINE_ENTITY_SEPARATOR;
	}
	tileData += Utilities::ONLINE_ENTITY_TYPE_SEPARATOR;

	m_onlineHandler.sendDataToClients(entityData + playerData + tileData);
}

void GameScreen::sendClientData()
{
	std::string data;
	//change to amount of host's local players once logic is in for that
	//for (int i = 0; i < 3; i++)
	{
		glm::ivec2 pos = static_cast<TransformComponent*>(m_players[0].getComponent(ComponentType::Transform))->getPos();
		data += std::to_string(pos.x);
		data += Utilities::ONLINE_ENTITY_DATA_SEPARATOR;
		data += std::to_string(pos.y);
		data += Utilities::ONLINE_ENTITY_DATA_SEPARATOR;
		data += std::to_string(static_cast<int>(static_cast<HealthComponent*>(m_players[0].getComponent(ComponentType::Health))->getHealth()));
		data += Utilities::ONLINE_ENTITY_SEPARATOR;
	}
	m_onlineHandler.sendDataToHost(data);
}
