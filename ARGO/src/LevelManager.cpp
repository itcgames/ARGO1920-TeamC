#include "stdafx.h"
#include "LevelManager.h"

LevelManager::LevelManager(SDL_Renderer* t_renderer, Entity(&t_players)[Utilities::S_MAX_PLAYERS], RenderSystem& t_renderSystem, ProjectileManager& t_projectileManager):
	m_renderer(t_renderer),
	m_players(t_players),
	m_renderSystem(t_renderSystem),
	m_projectilemanager(t_projectileManager)
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
			m_levelTiles.back().addComponent(new lightFieldComponent());
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
	resetFields();
	generateFlowField();
	generateLightField();
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

void LevelManager::renderLight(SDL_Renderer* t_renderer, RenderSystem* t_system)
{
	for (auto& entity : m_levelTiles)
	{
		t_system->renderLight(t_renderer, entity);
	}
}

void LevelManager::setToWall(Entity& t_entity)
{
	t_entity.removeCompType(ComponentType::Visual);
	t_entity.removeCompType(ComponentType::ColliderAABB);
	t_entity.removeCompType(ComponentType::Pathing);

	t_entity.addComponent(new VisualComponent("wall_4.png", m_renderer));
	t_entity.addComponent(new ColliderAABBComponent(glm::vec2(Utilities::TILE_SIZE, Utilities::TILE_SIZE)));
	static_cast<HealthComponent*>(t_entity.getComponent(ComponentType::Health))->setHealth(Utilities::WALL_HEALTH);
}

void LevelManager::setToFloor(Entity& t_entity)
{
	t_entity.removeCompType(ComponentType::Visual);
	t_entity.removeCompType(ComponentType::ColliderAABB);
	t_entity.removeCompType(ComponentType::Pathing);

	t_entity.addComponent(new VisualComponent("floor_1b.png", m_renderer));
	t_entity.addComponent(new PathingComponent());
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

void LevelManager::resetFields()
{
	for (auto& tile : m_levelTiles)
	{
		static_cast<FlowFieldComponent*>(tile.getComponent(ComponentType::FlowField))->reset();
		static_cast<lightFieldComponent*>(tile.getComponent(ComponentType::LightField))->reset();
	}
}

void LevelManager::generateFlowField()
{
	std::vector<Entity*> queue;
	for (auto& player : m_players)
	{
		HealthComponent* healthComp = static_cast<HealthComponent*>(player.getComponent(ComponentType::Health));
		TransformComponent* transformComp = static_cast<TransformComponent*>(player.getComponent(ComponentType::Transform));
		if (healthComp && transformComp && healthComp->isAlive())
		{
			Entity* tile = findAtPosition(transformComp->getPos());
			if (tile)
			{
				FlowFieldComponent* flowFieldComp = static_cast<FlowFieldComponent*>(tile->getComponent(ComponentType::FlowField));
				if (flowFieldComp)
				{
					setTileWeight(tile, nullptr, queue, 0);
				}
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

void LevelManager::generateLightField()
{
	std::vector<Entity*> queue;
	for (auto& player : m_players)
	{
		HealthComponent* healthComp = static_cast<HealthComponent*>(player.getComponent(ComponentType::Health));
		TransformComponent* transformComp = static_cast<TransformComponent*>(player.getComponent(ComponentType::Transform));
		if (healthComp && transformComp && healthComp->isAlive())
		{
			Entity* tile = findAtPosition(transformComp->getPos());
			if (tile)
			{
				FlowFieldComponent* flowFieldComp = static_cast<FlowFieldComponent*>(tile->getComponent(ComponentType::FlowField));
				if (flowFieldComp)
				{
					setTileLight(tile, queue, 0);
				}
			}
		}
	}

	for (auto& glowStick : m_projectilemanager.getGlowsticks())
	{
		HealthComponent* healthComp = static_cast<HealthComponent*>(glowStick.getComponent(ComponentType::Health));
		TransformComponent* transformComp = static_cast<TransformComponent*>(glowStick.getComponent(ComponentType::Transform));
		if (healthComp && transformComp && healthComp->isAlive())
		{
			Entity* tile = findAtPosition(transformComp->getPos());
			if (tile)
			{
				FlowFieldComponent* flowFieldComp = static_cast<FlowFieldComponent*>(tile->getComponent(ComponentType::FlowField));
				if (flowFieldComp)
				{
					setTileLight(tile, queue, 0);
				}
			}
		}
	}

	while (!queue.empty())
	{
		Entity* current = queue.back();
		queue.pop_back();
		setNeighbourLights(current, queue);
	}
}

void LevelManager::setTileLight(Entity* t_entity, std::vector<Entity*>& t_queue, int t_newWeight)
{
	lightFieldComponent* lightField = static_cast<lightFieldComponent*>(t_entity->getComponent(ComponentType::LightField));
	TransformComponent* transform = static_cast<TransformComponent*>(t_entity->getComponent(ComponentType::Transform));
	if (lightField && lightField->getWeight() > t_newWeight && transform)
	{
		lightField->setWeight(t_newWeight);
		if (!static_cast<ColliderAABBComponent*>(t_entity->getComponent(ComponentType::ColliderAABB)) && t_newWeight < MAX_LIGHT_WEIGHT)
		{
			t_queue.insert(t_queue.begin(), t_entity);
		}
	}
}

void LevelManager::setNeighbourLights(Entity* t_entity, std::vector<Entity*>& t_queue)
{
	lightFieldComponent* lightComp = static_cast<lightFieldComponent*>(t_entity->getComponent(ComponentType::LightField));
	Neighbours* neighbours = static_cast<TileComponent*>(t_entity->getComponent(ComponentType::Tile))->getNeighbours();
	int newWeight = lightComp->getWeight() + LIGHT_LOST_PER_TILE;
	int newCornerWeight = lightComp->getWeight() + LIGHT_LOST_PER_CORNER;

	if (neighbours->top)
	{
		setTileLight(neighbours->top, t_queue, newWeight);
	}
	if (neighbours->left)
	{
		setTileLight(neighbours->left, t_queue, newWeight);
	}
	if (neighbours->right)
	{
		setTileLight(neighbours->right, t_queue, newWeight);
	}
	if (neighbours->bottom)
	{
		setTileLight(neighbours->bottom, t_queue, newWeight);
	}
	if (neighbours->topLeft)
	{
		setTileLight(neighbours->topLeft, t_queue, newCornerWeight);
	}
	if (neighbours->topRight)
	{
		setTileLight(neighbours->topRight, t_queue, newCornerWeight);
	}
	if (neighbours->bottomLeft)
	{
		setTileLight(neighbours->bottomLeft, t_queue, newCornerWeight);
	}
	if (neighbours->bottomRight)
	{
		setTileLight(neighbours->bottomRight, t_queue, newCornerWeight);
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
		if (neighbours->topLeft && neighbours->top && neighbours->left && 
			!static_cast<ColliderAABBComponent*>(neighbours->top->getComponent(ComponentType::ColliderAABB)) &&
			!static_cast<ColliderAABBComponent*>(neighbours->left->getComponent(ComponentType::ColliderAABB)))
		{
			setTileWeight(neighbours->topLeft, t_entity, t_queue, newWeight);
		}
		if (neighbours->topRight && neighbours->top && neighbours->right &&
			!static_cast<ColliderAABBComponent*>(neighbours->top->getComponent(ComponentType::ColliderAABB)) &&
			!static_cast<ColliderAABBComponent*>(neighbours->right->getComponent(ComponentType::ColliderAABB)))
		{
			setTileWeight(neighbours->topRight, t_entity, t_queue, newWeight);
		}
		if (neighbours->bottomLeft && neighbours->bottom && neighbours->left &&
			!static_cast<ColliderAABBComponent*>(neighbours->bottom->getComponent(ComponentType::ColliderAABB)) &&
			!static_cast<ColliderAABBComponent*>(neighbours->left->getComponent(ComponentType::ColliderAABB)))
		{
			setTileWeight(neighbours->bottomLeft, t_entity, t_queue, newWeight);
		}
		if (neighbours->bottomRight && neighbours->bottom && neighbours->right &&
			!static_cast<ColliderAABBComponent*>(neighbours->bottom->getComponent(ComponentType::ColliderAABB)) &&
			!static_cast<ColliderAABBComponent*>(neighbours->right->getComponent(ComponentType::ColliderAABB)))
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

	if (x >= 0 && x < Utilities::LEVEL_TILE_WIDTH &&
		y >= 0 && y < Utilities::LEVEL_TILE_HEIGHT)
	{
		return &m_levelTiles[(y * Utilities::LEVEL_TILE_WIDTH) + x];
	}
	return nullptr;
}

void LevelManager::resetPathing()
{
	for (auto& tile : m_levelTiles)
	{
		PathingComponent* comp = static_cast<PathingComponent*>(tile.getComponent(ComponentType::Pathing));
		if (comp)
		{
			comp->reset();
		}
	}
}


std::vector<glm::vec2> LevelManager::createPath(glm::vec2 start, glm::vec2 target)
{
	resetPathing();

	Entity* startTile = findAtPosition(start);
	Entity* targetTile = findAtPosition(target);

	if (!targetTile || !startTile)
	{
		std::vector<glm::vec2> empty;
		return empty;
	}

	PathingComponent* startData = static_cast<PathingComponent*>(startTile->getComponent(ComponentType::Pathing));
	PathingComponent* targetData = static_cast<PathingComponent*>(targetTile->getComponent(ComponentType::Pathing));
	TransformComponent* startTransform = static_cast<TransformComponent*>(startTile->getComponent(ComponentType::Transform));
	TransformComponent* endTransform = static_cast<TransformComponent*>(targetTile->getComponent(ComponentType::Transform));

	if (!targetData || !startData)
	{
		std::vector<glm::vec2> empty;
		return empty;
	}
	startData->setDistance(0);
	startData->setTotalDistance(glm::distance2(startTransform->getPos(), endTransform->getPos()));

	std::priority_queue<Entity*, std::vector<Entity*>, LessThanByTotalDistance> queue;
	queue.push(startTile);

	while (!queue.empty() && queue.top() != targetTile)
	{
		Entity* current = queue.top();
		queue.pop();

		Neighbours* neighbours = static_cast<TileComponent*>(current->getComponent(ComponentType::Tile))->getNeighbours();

		if (neighbours->top)
		{
			addToPath(neighbours->top, current, endTransform->getPos(), &queue);
		}
		if (neighbours->left)
		{
			addToPath(neighbours->left, current, endTransform->getPos(), &queue);
		}
		if (neighbours->right)
		{
			addToPath(neighbours->right, current, endTransform->getPos(), &queue);
		}
		if (neighbours->bottom)
		{
			addToPath(neighbours->bottom, current, endTransform->getPos(), &queue);
		}
		if (neighbours->topLeft && neighbours->top && neighbours->left &&
			!static_cast<ColliderAABBComponent*>(neighbours->top->getComponent(ComponentType::ColliderAABB)) &&
			!static_cast<ColliderAABBComponent*>(neighbours->left->getComponent(ComponentType::ColliderAABB)))
		{
			addToPath(neighbours->topLeft, current, endTransform->getPos(), &queue);
		}
		if (neighbours->topRight && neighbours->top && neighbours->right &&
			!static_cast<ColliderAABBComponent*>(neighbours->top->getComponent(ComponentType::ColliderAABB)) &&
			!static_cast<ColliderAABBComponent*>(neighbours->right->getComponent(ComponentType::ColliderAABB)))
		{
			addToPath(neighbours->topRight, current, endTransform->getPos(), &queue);
		}
		if (neighbours->bottomLeft && neighbours->bottom && neighbours->left &&
			!static_cast<ColliderAABBComponent*>(neighbours->bottom->getComponent(ComponentType::ColliderAABB)) &&
			!static_cast<ColliderAABBComponent*>(neighbours->left->getComponent(ComponentType::ColliderAABB)))
		{
			addToPath(neighbours->bottomLeft, current, endTransform->getPos(), &queue);
		}
		if (neighbours->bottomRight && neighbours->bottom && neighbours->right &&
			!static_cast<ColliderAABBComponent*>(neighbours->bottom->getComponent(ComponentType::ColliderAABB)) &&
			!static_cast<ColliderAABBComponent*>(neighbours->right->getComponent(ComponentType::ColliderAABB)))
		{
			addToPath(neighbours->bottomRight, current, endTransform->getPos(), &queue);
		}
	}

	std::vector<glm::vec2> output;
	output.push_back(endTransform->getPos() + glm::vec2(Utilities::TILE_SIZE / 2, Utilities::TILE_SIZE / 2)); //position + tile centre
	while (targetData->getPrevious() != nullptr)
	{
		targetTile = targetData->getPrevious();
		targetData = static_cast<PathingComponent*>(targetTile->getComponent(ComponentType::Pathing));
		endTransform = static_cast<TransformComponent*>(targetTile->getComponent(ComponentType::Transform));
		output.push_back(endTransform->getPos() + glm::vec2(Utilities::TILE_SIZE / 2, Utilities::TILE_SIZE / 2));
	}

	output.pop_back();

	return output;
}

void LevelManager::addToPath(Entity* t_child, Entity* t_parent, glm::vec2 targetPos, std::priority_queue<Entity*, std::vector<Entity*>, LessThanByTotalDistance>* t_queue)
{
	PathingComponent* childData = static_cast<PathingComponent*>(t_child->getComponent(ComponentType::Pathing));
	PathingComponent* parentData = static_cast<PathingComponent*>(t_parent->getComponent(ComponentType::Pathing));
	TransformComponent* childTransform = static_cast<TransformComponent*>(t_child->getComponent(ComponentType::Transform));
	TransformComponent* parentTransform = static_cast<TransformComponent*>(t_parent->getComponent(ComponentType::Transform));

	if (childData)
	{
		float distance = glm::distance2(childTransform->getPos(), parentTransform->getPos()) + parentData->getDistance();
		if (childData->getDistance() > distance)
		{
			childData->setDistance(distance);
			childData->setTotalDistance(distance + glm::distance2(childTransform->getPos(), targetPos));
			childData->setPrevious(t_parent);

			t_queue->push(t_child);
		}
	}
}

