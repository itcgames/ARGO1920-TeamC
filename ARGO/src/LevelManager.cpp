#include "stdafx.h"
#include "LevelManager.h"

LevelManager::LevelManager(SDL_Renderer* t_renderer):
	m_renderer(t_renderer)
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
			m_levelTiles.back().addComponent(new VisualComponent("wall_4.png", m_renderer));
			m_levelTiles.back().addComponent(new ColliderAABBComponent(glm::vec2(Utilities::TILE_SIZE, Utilities::TILE_SIZE)));
			m_levelTiles.back().addComponent(new TileComponent());
			m_levelTiles.back().addComponent(new HealthComponent(Utilities::WALL_HEALTH, Utilities::WALL_HEALTH));
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

