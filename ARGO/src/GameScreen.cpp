#include "stdafx.h"
#include "GameScreen.h"
#include "..\include\GameScreen.h"

GameScreen::GameScreen(SDL_Renderer* t_renderer, MenuStates* t_currentScreen, EventManager& t_eventManager) :
	m_renderer{ t_renderer },
	m_currentScreen{ t_currentScreen },
	m_eventManager{ t_eventManager },
	m_transformSystem{ m_eventManager },
	m_projectileManager{ m_eventManager }
{
	createButtonMaps();
	for (Entity& player : m_players)
	{
		createPlayer(player);
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
}

void GameScreen::update(bool t_canTick, bool t_canRender, Uint16 t_deltaTime)
{
	preRender();
	updateLevelTiles(t_canTick, t_canRender);
	updateEntities(t_canTick, t_canRender);
	updatePlayers(t_canTick, t_canRender);
	updateProjectiles(t_canTick, t_canRender);
	if (t_canTick) m_collisionSystem.handleCollisions();
}

void GameScreen::processEvents(SDL_Event* t_event)
{
	switch (t_event->type)
	{
	case SDL_KEYDOWN:
	{
		switch (t_event->key.keysym.sym)
		{
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

void GameScreen::createButtonMaps()
{
	using ButtonCommandPair = std::pair<ButtonType, Command*>;
	m_buttonPressMap =
	{
		ButtonCommandPair(ButtonType::DpadUp, new MoveUpCommand()),
		ButtonCommandPair(ButtonType::DpadDown, new MoveDownCommand()),
		ButtonCommandPair(ButtonType::DpadLeft, new MoveLeftCommand()),
		ButtonCommandPair(ButtonType::DpadRight, new MoveRightCommand()),
		ButtonCommandPair(ButtonType::Back, new CloseWindowCommand())
	};
	// Set Held To Same as Pressed Commands For Time Being
	m_buttonHeldMap = m_buttonPressMap;
	// Set Release Commands to nothing
	m_buttonReleasedMap = std::map<ButtonType, Command*>();
}

void GameScreen::createPlayer(Entity& t_player)
{
	t_player.addComponent(new HealthComponent(10, 10));
	t_player.addComponent(new TransformComponent());
	t_player.addComponent(new InputComponent(m_buttonPressMap, m_buttonHeldMap, m_buttonReleasedMap));
	t_player.addComponent(new ForceComponent());
	t_player.addComponent(new ColliderCircleComponent(Utilities::PLAYER_RADIUS));
	t_player.addComponent(new ColourComponent(glm::linearRand(0, 255), glm::linearRand(0, 255), glm::linearRand(0, 255), 255));
	t_player.addComponent(new CommandComponent());
	t_player.addComponent(new TagComponent(Tag::Player));
}


void GameScreen::createEnemy()
{
	m_entities.emplace_back(); 
	m_entities.back().addComponent(new TransformComponent());
	m_entities.back().addComponent(new AiComponent());
	m_entities.back().addComponent(new ColliderCircleComponent(Utilities::ENEMY_RADIUS));
	m_entities.back().addComponent(new TagComponent(Tag::Enemy));
}

void GameScreen::setUpLevel()
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

void GameScreen::updatePlayers(bool t_canTick, bool t_canRender)
{
	for (Entity& player : m_players)
	{
		if (t_canTick)
		{
			m_inputSystem.update(player);
			m_commandSystem.update(player, m_eventManager);
			m_healthSystem.update(player);
			m_transformSystem.update(player);
			m_collisionSystem.update(player);
		}
		if (t_canRender)
		{
			m_renderSystem.render(m_renderer, player);
		}
	}
}

void GameScreen::updateEntities(bool t_canTick, bool t_canRender)
{
	for (Entity& entity : m_entities)
	{
		if (t_canTick)
		{
			m_healthSystem.update(entity);
			m_aiSystem.update(entity);
			m_transformSystem.update(entity);
			m_collisionSystem.update(entity);
		}
		if (t_canRender)
		{
			m_renderSystem.render(m_renderer, entity);
		}
	}
}

void GameScreen::updateLevelTiles(bool t_canTick, bool t_canRender)
{
	for (Entity& tileEntity : m_levelTiles)
	{
		if (t_canTick)
		{
			m_collisionSystem.update(tileEntity);
		}
		if (t_canRender)
		{
			m_renderSystem.render(m_renderer, tileEntity);
		}
	}
}

void GameScreen::updateProjectiles(bool t_canTick, bool t_canRender)
{
	if (t_canTick)
	{
		m_projectileManager.update(&m_transformSystem);
		m_projectileManager.update(&m_collisionSystem);
	}
	if (t_canRender)
	{
		m_projectileManager.render(m_renderer, &m_renderSystem);
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

void GameScreen::setToFloor(Entity& t_entity, glm::vec2 t_position)
{
	t_entity.removeAllComponents();
	t_entity.addComponent(new TransformComponent(t_position));
	t_entity.addComponent(new VisualComponent("floor_1b.png", m_renderer)); //TODO: change to floor texture when assets have been recieved.
}

void GameScreen::setToWall(Entity& t_entity, glm::vec2 t_position)
{
	t_entity.removeAllComponents();
	t_entity.addComponent(new TransformComponent(t_position));
	t_entity.addComponent(new VisualComponent("wall_4.png", m_renderer)); //TODO: change to wall texture when assets have been recieved.
	t_entity.addComponent(new ColliderAABBComponent(glm::vec2(Utilities::TILE_SIZE, Utilities::TILE_SIZE)));
	t_entity.addComponent(new TagComponent(Tag::Wall));
}
