#include "stdafx.h"
#include "CollisionSystem.h"

CollisionSystem::CollisionSystem() :
	m_quadTree(0, glm::vec2(0, 0), glm::vec2(Utilities::LEVEL_TILE_WIDTH * Utilities::TILE_SIZE, Utilities::LEVEL_TILE_HEIGHT * Utilities::TILE_SIZE)),
	m_seperationScaler(30)
{
	m_circleColliderBuffer.reserve(100);
}

void CollisionSystem::update(Entity& t_entity)
{
	//if not nullptr
	if (t_entity.getAllComps().at(COMPONENT_ID::COLLIDER_AABB_ID) &&
		t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))
	{
		glm::vec2 rectPosition = static_cast<TransformComponent*>(t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos();
		glm::vec2 bounds = static_cast<ColliderAABBComponent*>(t_entity.getAllComps().at(COMPONENT_ID::COLLIDER_AABB_ID))->getBounds();
		Quad quad{ &t_entity, rectPosition, bounds };
		m_quadTree.insert(quad);
	}
	else if (t_entity.getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID) &&
		t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))
	{
		m_circleColliderBuffer.push_back(&t_entity);
		glm::vec2 rectPosition = static_cast<TransformComponent*>(t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos();
		int radius = static_cast<ColliderCircleComponent*>(t_entity.getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();
		glm::vec2 bounds{ radius, radius };
		Quad quad{ &t_entity, rectPosition - bounds, bounds * 2.0f };
		m_quadTree.insert(quad);
	}
}

void CollisionSystem::handleCollisions()
{
	for (int i = 0; i < m_circleColliderBuffer.size(); i++)
	{
		TagComponent* tag = static_cast<TagComponent*>(m_circleColliderBuffer[i]->getAllComps().at(COMPONENT_ID::TAG_ID));

		switch (tag->getTag())
		{
		case Tag::Player:
			handlePlayerCollision(m_circleColliderBuffer[i]);
			break;
		case Tag::Enemy:
			handleEnemyCollision(m_circleColliderBuffer[i]);
			break;
		case Tag::PlayerBullet:
			handlePlayerBulletCollision(m_circleColliderBuffer[i]);
			break;
		case Tag::EnemyBullet:
			handleEnemyBulletCollision(m_circleColliderBuffer[i]);
			break;
		default:
			break;
		}
	}
	m_circleColliderBuffer.clear();
	m_quadTree.clear();
}

bool CollisionSystem::circleToCircleCollision(Entity* t_entityCircle1, Entity* t_entityCircle2)
{
	glm::vec2 position1 = static_cast<TransformComponent*>(t_entityCircle1->getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos();
	int radius1 = static_cast<ColliderCircleComponent*>(t_entityCircle1->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();
	glm::vec2 position2 = static_cast<TransformComponent*>(t_entityCircle2->getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos();
	int radius2 = static_cast<ColliderCircleComponent*>(t_entityCircle2->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();

	glm::vec2 distanceBetween = position1 - position2;
	return sqrt(distanceBetween.x * distanceBetween.x + distanceBetween.y * distanceBetween.y) < radius1 + radius2;
}

bool CollisionSystem::circleToAABBCollision(Entity* t_entityCircle, Entity* t_entityAABB)
{
	glm::vec2 circlePosition = static_cast<TransformComponent*>(t_entityCircle->getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos();
	int radius = static_cast<ColliderCircleComponent*>(t_entityCircle->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();
	glm::vec2 rectPosition = static_cast<TransformComponent*>(t_entityAABB->getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos();
	glm::vec2 bounds = static_cast<ColliderAABBComponent*>(t_entityAABB->getAllComps().at(COMPONENT_ID::COLLIDER_AABB_ID))->getBounds();

	int deltaX = circlePosition.x - std::max(rectPosition.x, std::min(circlePosition.x, rectPosition.x + bounds.x));
	int deltaY = circlePosition.y - std::max(rectPosition.y, std::min(circlePosition.y, rectPosition.y + bounds.y));
	return (deltaX * deltaX + deltaY * deltaY) < (radius * radius);
}

void CollisionSystem::handlePlayerCollision(Entity* t_player)
{
	TransformComponent* rectPosition = static_cast<TransformComponent*>(t_player->getAllComps().at(COMPONENT_ID::TRANSFORM_ID));
	int radius = static_cast<ColliderCircleComponent*>(t_player->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();
	glm::vec2 bounds{ radius, radius };
	Quad quad{ NULL, rectPosition->getPos() - bounds, bounds * 2.0f };

	std::vector<Entity*> entities;
	m_quadTree.retrieve(&entities, quad);

	for (auto& other : entities)
	{
		TagComponent* tag = static_cast<TagComponent*>(other->getAllComps().at(COMPONENT_ID::TAG_ID));

		switch (tag->getTag())
		{
		case Tag::Player:
		{
			if (other != t_player)
			{
				playerToPlayer(t_player, other);
			}
		}
			break;
		case Tag::Enemy:
			playerToEnemy(t_player, other);
			break;
		case Tag::EnemyBullet:
			playerToEnemyBullet(t_player, other);
			break;
		case Tag::Tile:
			playerToWall(t_player, other);
			break;
		default:
			break;
		}
	}
}

void CollisionSystem::handlePlayerBulletCollision(Entity* t_playerBullet)
{
	TransformComponent* rectPosition = static_cast<TransformComponent*>(t_playerBullet->getAllComps().at(COMPONENT_ID::TRANSFORM_ID));
	int radius = static_cast<ColliderCircleComponent*>(t_playerBullet->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();
	glm::vec2 bounds{ radius, radius };
	Quad quad{ NULL, rectPosition->getPos() - bounds, bounds * 2.0f };

	std::vector<Entity*> entities;
	m_quadTree.retrieve(&entities, quad);

	for (auto& other : entities)
	{
		TagComponent* tag = static_cast<TagComponent*>(other->getAllComps().at(COMPONENT_ID::TAG_ID));

		switch (tag->getTag())
		{
		case Tag::Enemy:
			playerBulletToEnemy(t_playerBullet, other);
			break;
		case Tag::Tile:
			playerBulletToWall(t_playerBullet, other);
			break;
		default:
			break;
		}
	}
}

void CollisionSystem::handleEnemyCollision(Entity* t_enemy)
{
	TransformComponent* rectPosition = static_cast<TransformComponent*>(t_enemy->getAllComps().at(COMPONENT_ID::TRANSFORM_ID));
	int radius = static_cast<ColliderCircleComponent*>(t_enemy->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();
	glm::vec2 bounds{ radius, radius };
	Quad quad{ NULL, rectPosition->getPos() - bounds, bounds * 2.0f };

	std::vector<Entity*> entities;
	m_quadTree.retrieve(&entities, quad);

	for (auto& other : entities)
	{
		TagComponent* tag = static_cast<TagComponent*>(other->getAllComps().at(COMPONENT_ID::TAG_ID));

		switch (tag->getTag())
		{
		case Tag::Enemy:
		{
			if (other != t_enemy)
			{
				enemyToEnemy(t_enemy, other);
			}
		}
			break;
		case Tag::Tile:
			enemyToWall(t_enemy, other);
			break;
		default:
			break;
		}
	}
}

void CollisionSystem::handleEnemyBulletCollision(Entity* t_enemyBullet)
{
}

void CollisionSystem::playerToPlayer(Entity* t_player1, Entity* t_player2)
{
	if (t_player2->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID) && circleToCircleCollision(t_player1, t_player2))
	{
		int player1Radius = static_cast<ColliderCircleComponent*>(t_player1->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();
		TransformComponent* player1Position = static_cast<TransformComponent*>(t_player1->getAllComps().at(COMPONENT_ID::TRANSFORM_ID));

		int player2Radius = static_cast<ColliderCircleComponent*>(t_player2->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();
		TransformComponent* player2Position = static_cast<TransformComponent*>(t_player2->getAllComps().at(COMPONENT_ID::TRANSFORM_ID));

		glm::vec2 distanceBetween = player1Position->getPos() - player2Position->getPos();
		float length = sqrt(distanceBetween.x * distanceBetween.x + distanceBetween.y * distanceBetween.y);
		float seperation = 1 - length / (player1Radius + player2Radius);
		if (length != 0)
		{
			distanceBetween /= length;
			distanceBetween = distanceBetween * seperation * m_seperationScaler;
		}
		else
		{
			distanceBetween = glm::vec2(m_seperationScaler, m_seperationScaler);
		}
		player1Position->addPos(distanceBetween);
		player2Position->addPos(-distanceBetween);
	}
}

void CollisionSystem::playerToEnemy(Entity* t_player, Entity* t_enemy)
{
	if (t_enemy->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID) && circleToCircleCollision(t_player, t_enemy))
	{
		int player1Radius = static_cast<ColliderCircleComponent*>(t_player->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();
		TransformComponent* player1Position = static_cast<TransformComponent*>(t_player->getAllComps().at(COMPONENT_ID::TRANSFORM_ID));

		int player2Radius = static_cast<ColliderCircleComponent*>(t_enemy->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();
		TransformComponent* player2Position = static_cast<TransformComponent*>(t_enemy->getAllComps().at(COMPONENT_ID::TRANSFORM_ID));

		glm::vec2 distanceBetween = player1Position->getPos() - player2Position->getPos();
		float length = sqrt(distanceBetween.x * distanceBetween.x + distanceBetween.y * distanceBetween.y);
		float seperation = 1 - length / (player1Radius + player2Radius);
		if (length != 0)
		{
			distanceBetween /= length;
			distanceBetween = distanceBetween * seperation * m_seperationScaler;
		}
		else
		{
			distanceBetween = glm::vec2(m_seperationScaler, m_seperationScaler);
		}
		player1Position->addPos(distanceBetween);
		player2Position->addPos(-distanceBetween);
	}
}

void CollisionSystem::playerToEnemyBullet(Entity* t_player, Entity* t_enemyBullet)
{
	//TODO
}

void CollisionSystem::playerToWall(Entity* t_player, Entity* t_wall)
{
	if (t_wall->getAllComps().at(COMPONENT_ID::COLLIDER_AABB_ID) && circleToAABBCollision(t_player, t_wall))
	{
		int playerRadius = static_cast<ColliderCircleComponent*>(t_player->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();
		TransformComponent* playerPosition = static_cast<TransformComponent*>(t_player->getAllComps().at(COMPONENT_ID::TRANSFORM_ID));

		int wallWidth = static_cast<ColliderAABBComponent*>(t_wall->getAllComps().at(COMPONENT_ID::COLLIDER_AABB_ID))->getBounds().x;
		TransformComponent* wallPosition = static_cast<TransformComponent*>(t_wall->getAllComps().at(COMPONENT_ID::TRANSFORM_ID));

		glm::vec2 distanceBetween = playerPosition->getPos() - glm::vec2(playerRadius, playerRadius) - wallPosition->getPos();
		
		if (distanceBetween.x < playerRadius * 2 + wallWidth && (std::abs(distanceBetween.x) > std::abs(distanceBetween.y)))
		{
			if (distanceBetween.x < 0)
			{
				playerPosition->setPos(-(playerRadius) + wallPosition->getPos().x, playerPosition->getPos().y);
			}
			else
			{
				playerPosition->setPos((playerRadius + wallWidth) + wallPosition->getPos().x, playerPosition->getPos().y);
			}
		}
		if (distanceBetween.y < playerRadius * 2 + wallWidth && (std::abs(distanceBetween.x) < std::abs(distanceBetween.y)))
		{
			if (distanceBetween.y < 0)
			{
				playerPosition->setPos(playerPosition->getPos().x, -(playerRadius)+wallPosition->getPos().y);
			}
			else
			{
				playerPosition->setPos(playerPosition->getPos().x, (playerRadius + wallWidth) + wallPosition->getPos().y);
			}
		}


		float length = sqrt(distanceBetween.x * distanceBetween.x + distanceBetween.y * distanceBetween.y);
		float seperation = 1 - length / (playerRadius + wallWidth);
		if (length != 0)
		{
			distanceBetween /= length;
			distanceBetween = distanceBetween * seperation * m_seperationScaler;
		}
		else
		{
			distanceBetween = glm::vec2(m_seperationScaler, m_seperationScaler);
		}
		//playerPosition->addPos(distanceBetween);
	}
}

void CollisionSystem::playerBulletToEnemy(Entity* t_playerBullet, Entity* t_enemy)
{
	if (t_enemy->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID) && circleToCircleCollision(t_playerBullet, t_enemy))
	{
		static_cast<HealthComponent*>(t_playerBullet->getAllComps().at(COMPONENT_ID::HEALTH_ID))->setHealth(0); //kill the bullet
	}
}

void CollisionSystem::playerBulletToWall(Entity* t_playerBullet, Entity* t_wall)
{
	if (t_wall->getAllComps().at(COMPONENT_ID::COLLIDER_AABB_ID) && circleToAABBCollision(t_playerBullet, t_wall))
	{
		static_cast<HealthComponent*>(t_playerBullet->getAllComps().at(COMPONENT_ID::HEALTH_ID))->setHealth(0); //kill the bullet
	}
}

void CollisionSystem::enemyToEnemy(Entity* t_enemy1, Entity* t_enemy2)
{
	if (t_enemy2->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID) && circleToCircleCollision(t_enemy1, t_enemy2))
	{
		int enemy1Radius = static_cast<ColliderCircleComponent*>(t_enemy1->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();
		TransformComponent* enemy1Position = static_cast<TransformComponent*>(t_enemy1->getAllComps().at(COMPONENT_ID::TRANSFORM_ID));

		int player2Radius = static_cast<ColliderCircleComponent*>(t_enemy2->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();
		TransformComponent* enemy2Position = static_cast<TransformComponent*>(t_enemy2->getAllComps().at(COMPONENT_ID::TRANSFORM_ID));

		glm::vec2 distanceBetween = enemy1Position->getPos() - enemy2Position->getPos();
		float length = sqrt(distanceBetween.x * distanceBetween.x + distanceBetween.y * distanceBetween.y);
		float seperation = 1 - length / (enemy1Radius + player2Radius);
		if (length != 0)
		{
			distanceBetween /= length;
			distanceBetween = distanceBetween * seperation * m_seperationScaler;
		}
		else
		{
			distanceBetween = glm::vec2(m_seperationScaler, m_seperationScaler);
		}
		enemy1Position->addPos(distanceBetween);
		enemy2Position->addPos(-distanceBetween);
	}
}

void CollisionSystem::enemyToWall(Entity* t_enemy, Entity* t_wall)
{
	if (t_wall->getAllComps().at(COMPONENT_ID::COLLIDER_AABB_ID) && circleToAABBCollision(t_enemy, t_wall))
	{
		//handle aabb to circle collisisons here
	}
}

void CollisionSystem::enemyBulletToWall(Entity* t_enemyBullet, Entity* t_wall)
{
	if (t_wall->getAllComps().at(COMPONENT_ID::COLLIDER_AABB_ID) && circleToAABBCollision(t_enemyBullet, t_wall))
	{
		static_cast<HealthComponent*>(t_enemyBullet->getAllComps().at(COMPONENT_ID::HEALTH_ID))->setHealth(0); //kill the bullet
	}
}
