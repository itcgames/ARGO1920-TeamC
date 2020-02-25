#include "stdafx.h"
#include "LevelManager.h"

LevelManager::LevelManager(SDL_Renderer* t_renderer, Entity(&t_players)[Utilities::S_MAX_PLAYERS]):
	m_renderer(t_renderer),
	m_players(t_players)
{
}

void LevelManager::setupLevel()
{
	if (!m_levelTiles.empty())
	{
		for (Entity& tile : m_levelTiles)
		{
			tile.removeAllComponents();
		}
		m_levelTiles.clear();
	}
	m_levelTiles.reserve(Utilities::LEVEL_TILE_HEIGHT * Utilities::LEVEL_TILE_WIDTH);
	for (int i = 0; i < Utilities::LEVEL_TILE_HEIGHT; i++)
	{
		for (int j = 0; j < Utilities::LEVEL_TILE_WIDTH; j++)
		{
			m_levelTiles.emplace_back();

			m_levelTiles.back().addComponent(new TransformComponent(glm::vec2(j * Utilities::TILE_SIZE, i * Utilities::TILE_SIZE)));
			m_levelTiles.back().addComponent(new TagComponent(Tag::Tile));
			m_levelTiles.back().addComponent(new VisualComponent("wallSmall.png", m_renderer));
			m_levelTiles.back().addComponent(new ColliderAABBComponent(glm::vec2(Utilities::TILE_SIZE, Utilities::TILE_SIZE)));
			m_levelTiles.back().addComponent(new TileComponent());
			m_levelTiles.back().addComponent(new HealthComponent(Utilities::WALL_HEALTH, Utilities::WALL_HEALTH));
			m_levelTiles.back().addComponent(new FlowFieldComponent());
		}
	}
	setTileNeighbours();
} 

void LevelManager::update(BaseSystem* t_system)
{
	for (auto& entity : m_levelTiles)
	{
		if (entity.getComponent(ComponentType::ColliderAABB))
		{
			t_system->update(entity);
		}
	}
	generateFlowField();
}

void LevelManager::checkWallDamage()
{
	for (auto& entity : m_levelTiles)
	{
		if (entity.getComponent(ComponentType::ColliderAABB) && static_cast<HealthComponent*>(entity.getComponent(ComponentType::Health))->getHealth() <= 0)
		{
			setToFloor(entity);
		}
	}
}

void LevelManager::render(SDL_Renderer* t_renderer, RenderSystem* t_system)
{
	for (auto& entity : m_levelTiles)
	{
		t_system->render(t_renderer, entity);
	}
}

void LevelManager::setToWall(Entity& t_entity)
{
	t_entity.removeCompType(ComponentType::Visual);
	t_entity.removeCompType(ComponentType::ColliderAABB);

	t_entity.addComponent(new VisualComponent("wall_4.png", m_renderer));
	t_entity.addComponent(new ColliderAABBComponent(glm::vec2(Utilities::TILE_SIZE, Utilities::TILE_SIZE)));
	static_cast<HealthComponent*>(t_entity.getComponent(ComponentType::Health))->setHealth(Utilities::WALL_HEALTH);
}

void LevelManager::setToFloor(Entity& t_entity)
{
	t_entity.removeCompType(ComponentType::Visual);
	t_entity.removeCompType(ComponentType::ColliderAABB);

	t_entity.addComponent(new VisualComponent("floor_1b.png", m_renderer));
}

void LevelManager::createRoom(glm::vec2 t_startPosition, int t_width, int t_height)
{
	Entity* startTile = findAtPosition(t_startPosition * (float)Utilities::TILE_SIZE);
	for (int i = 0; i < t_width; i++)
	{
		Entity* currentTile = startTile;
		for (int j = 0; j < t_height; j++)
		{
			setToFloor(*currentTile);
			currentTile = static_cast<TileComponent*>(currentTile->getComponent(ComponentType::Tile))->getNeighbours()->bottom;
		}
		startTile = static_cast<TileComponent*>(startTile->getComponent(ComponentType::Tile))->getNeighbours()->right;
	}
}

void LevelManager::setTileNeighbours()
{
	for (auto& tile : m_levelTiles)
	{
		glm::vec2 position = static_cast<TransformComponent*>(tile.getComponent(ComponentType::Transform))->getPos();
		Neighbours* neighbours = static_cast<TileComponent*>(tile.getComponent(ComponentType::Tile))->getNeighbours();

		neighbours->top = findAtPosition(position + glm::vec2(0, -Utilities::TILE_SIZE));
		neighbours->left = findAtPosition(position + glm::vec2(-Utilities::TILE_SIZE, 0));
		neighbours->right = findAtPosition(position + glm::vec2(Utilities::TILE_SIZE, 0));
		neighbours->bottom = findAtPosition(position + glm::vec2(0, Utilities::TILE_SIZE));
		neighbours->topLeft = findAtPosition(position + glm::vec2(-Utilities::TILE_SIZE, -Utilities::TILE_SIZE));
		neighbours->topRight = findAtPosition(position + glm::vec2(Utilities::TILE_SIZE, -Utilities::TILE_SIZE));
		neighbours->bottomLeft = findAtPosition(position + glm::vec2(-Utilities::TILE_SIZE, Utilities::TILE_SIZE));
		neighbours->bottomRight = findAtPosition(position + glm::vec2(Utilities::TILE_SIZE, Utilities::TILE_SIZE));
	}
}

void LevelManager::resetFlowField()
{
	for (auto& tile : m_levelTiles)
	{
		static_cast<FlowFieldComponent*>(tile.getComponent(ComponentType::FlowField))->reset();
	}
}

void LevelManager::generateFlowField()
{
	resetFlowField();

	std::vector<Entity*> queue;
	for (auto& player : m_players)
	{
		HealthComponent* healthComp = static_cast<HealthComponent*>(player.getComponent(ComponentType::Health));
		TransformComponent* transformComp = static_cast<TransformComponent*>(player.getComponent(ComponentType::Transform));
		if (healthComp && transformComp && healthComp->isAlive())
		{
			Entity* tile = findAtPosition(transformComp->getPos());
			FlowFieldComponent* flowFieldComp = static_cast<FlowFieldComponent*>(tile->getComponent(ComponentType::FlowField));
			if (flowFieldComp)
			{
				setTileWeight(tile, nullptr, queue, 0);
			}
		}
	}

	while (!queue.empty())
	{
		Entity* current = queue.back();
		queue.pop_back();
		setNeighbourWeights(current, queue);
	}
}

void LevelManager::setNeighbourWeights(Entity* t_entity, std::vector<Entity*>& t_queue)
{
	int newWeight = static_cast<FlowFieldComponent*>(t_entity->getComponent(ComponentType::FlowField))->getWeight() + 1;
	if (newWeight < Utilities::MAX_FLOW_FIELD_WEIGHT)
	{
		Neighbours* neighbours = static_cast<TileComponent*>(t_entity->getComponent(ComponentType::Tile))->getNeighbours();
		if (neighbours->top)
		{
			setTileWeight(neighbours->top, t_entity, t_queue, newWeight);
		}
		if (neighbours->left)
		{
			setTileWeight(neighbours->left, t_entity, t_queue, newWeight);
		}
		if (neighbours->right)
		{
			setTileWeight(neighbours->right, t_entity, t_queue, newWeight);
		}
		if (neighbours->bottom)
		{
			setTileWeight(neighbours->bottom, t_entity, t_queue, newWeight);
		}
		if (neighbours->topLeft)
		{
			setTileWeight(neighbours->topLeft, t_entity, t_queue, newWeight);
		}
		if (neighbours->topRight)
		{
			setTileWeight(neighbours->topRight, t_entity, t_queue, newWeight);
		}
		if (neighbours->bottomLeft)
		{
			setTileWeight(neighbours->bottomLeft, t_entity, t_queue, newWeight);
		}
		if (neighbours->bottomRight)
		{
			setTileWeight(neighbours->bottomRight, t_entity, t_queue, newWeight);
		}
	}
}

void LevelManager::setTileWeight(Entity* t_entity, Entity* t_closestNeighbour, std::vector<Entity*>& t_queue, int t_newWeight)
{
	FlowFieldComponent* neighbourFlowField = static_cast<FlowFieldComponent*>(t_entity->getComponent(ComponentType::FlowField));
	if (!static_cast<ColliderAABBComponent*>(t_entity->getComponent(ComponentType::ColliderAABB)) && neighbourFlowField->getWeight() > t_newWeight)
	{
		neighbourFlowField->setClosestNeighbour(t_closestNeighbour);
		neighbourFlowField->setWeight(t_newWeight);
		t_queue.insert(t_queue.begin(), t_entity);
	}
}

Entity* LevelManager::findAtPosition(glm::vec2 t_position)
{
	int x = t_position.x / Utilities::TILE_SIZE;
	int y = t_position.y / Utilities::TILE_SIZE;

	if (x > 0 && x < Utilities::LEVEL_TILE_WIDTH &&
		y > 0 && y < Utilities::LEVEL_TILE_HEIGHT)
	{
		return &m_levelTiles[(y * Utilities::LEVEL_TILE_WIDTH) + x];
	}
	return nullptr;
}

