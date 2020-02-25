#include "stdafx.h"
#include "CollisionSystem.h"

CollisionSystem::CollisionSystem(EventManager& t_eventManager) :
	m_eventManager(t_eventManager),
	m_quadTree(0, glm::vec2(0, 0), glm::vec2(Utilities::LEVEL_TILE_WIDTH* Utilities::TILE_SIZE, Utilities::LEVEL_TILE_HEIGHT* Utilities::TILE_SIZE)),
	m_seperationScaler(30)
{
	m_circleColliderBuffer.reserve(100);
}

void CollisionSystem::update(Entity& t_entity)
{
	//if not nullptr
	TransformComponent* transComp = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform));
	ColliderAABBComponent* aabbComp = static_cast<ColliderAABBComponent*>(t_entity.getComponent(ComponentType::ColliderAABB));
	ColliderCircleComponent* circleComp = static_cast<ColliderCircleComponent*>(t_entity.getComponent(ComponentType::ColliderCircle));

	if (transComp)
	{
		if (aabbComp)
		{
			glm::vec2 rectPosition = transComp->getPos();
			glm::vec2 bounds = aabbComp->getBounds();
			Quad quad{ &t_entity, rectPosition, bounds };
			m_quadTree.insert(quad);
		}
		else if (circleComp && edgeOfTheWorldToCircle(t_entity))
		{
			m_circleColliderBuffer.push_back(&t_entity);
			glm::vec2 rectPosition = transComp->getPos();
			int radius = circleComp->getRadius();
			glm::vec2 bounds{ radius, radius };
			Quad quad{ &t_entity, rectPosition - bounds, bounds * 2.0f };
			m_quadTree.insert(quad);
		}
	}
}

void CollisionSystem::handleCollisions()
{
	for (int i = 0; i < m_circleColliderBuffer.size(); i++)
	{
		TagComponent* tag = static_cast<TagComponent*>(m_circleColliderBuffer[i]->getComponent(ComponentType::Tag));

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
	glm::vec2 position1 = static_cast<TransformComponent*>(t_entityCircle1->getComponent(ComponentType::Transform))->getPos();
	int radius1 = static_cast<ColliderCircleComponent*>(t_entityCircle1->getComponent(ComponentType::ColliderCircle))->getRadius();
	glm::vec2 position2 = static_cast<TransformComponent*>(t_entityCircle2->getComponent(ComponentType::Transform))->getPos();
	int radius2 = static_cast<ColliderCircleComponent*>(t_entityCircle2->getComponent(ComponentType::ColliderCircle))->getRadius();

	glm::vec2 distanceBetween = position1 - position2;
	return sqrt(distanceBetween.x * distanceBetween.x + distanceBetween.y * distanceBetween.y) < radius1 + radius2;
}

bool CollisionSystem::circleToAABBCollision(Entity* t_entityCircle, Entity* t_entityAABB)
{
	glm::vec2 circlePosition = static_cast<TransformComponent*>(t_entityCircle->getComponent(ComponentType::Transform))->getPos();
	int radius = static_cast<ColliderCircleComponent*>(t_entityCircle->getComponent(ComponentType::ColliderCircle))->getRadius();
	glm::vec2 rectPosition = static_cast<TransformComponent*>(t_entityAABB->getComponent(ComponentType::Transform))->getPos();
	glm::vec2 bounds = static_cast<ColliderAABBComponent*>(t_entityAABB->getComponent(ComponentType::ColliderAABB))->getBounds();

	int deltaX = circlePosition.x - std::max(rectPosition.x, std::min(circlePosition.x, rectPosition.x + bounds.x));
	int deltaY = circlePosition.y - std::max(rectPosition.y, std::min(circlePosition.y, rectPosition.y + bounds.y));
	return (deltaX * deltaX + deltaY * deltaY) < (radius * radius);
}

bool CollisionSystem::edgeOfTheWorldToCircle(Entity& t_entity)
{
	TransformComponent* position = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform));
	int radius = static_cast<ColliderCircleComponent*>(t_entity.getComponent(ComponentType::ColliderCircle))->getRadius();
	if (position->getPos().x - radius < 0)
	{
		if (!killBulletAtEdgeOfWorld(t_entity))
		{
			return false;
		}
		position->setPos(radius, position->getPos().y);
	}
	else if (position->getPos().x + radius > Utilities::TILE_SIZE* Utilities::LEVEL_TILE_WIDTH)
	{
		if (!killBulletAtEdgeOfWorld(t_entity))
		{
			return false;
		}
		position->setPos(Utilities::TILE_SIZE * Utilities::LEVEL_TILE_WIDTH - radius, position->getPos().y);
	}
	if (position->getPos().y - radius < 0)
	{
		if (!killBulletAtEdgeOfWorld(t_entity))
		{
			return false;
		}
		position->setPos(position->getPos().x, radius);
	}
	else if (position->getPos().y + radius > Utilities::TILE_SIZE* Utilities::LEVEL_TILE_HEIGHT)
	{
		if (!killBulletAtEdgeOfWorld(t_entity))
		{
			return false;
		}
		position->setPos(position->getPos().x, Utilities::TILE_SIZE * Utilities::LEVEL_TILE_HEIGHT - radius);
	}
	return true;
}

bool CollisionSystem::killBulletAtEdgeOfWorld(Entity& t_entity)
{
	Tag tag = static_cast<TagComponent*>(t_entity.getComponent(ComponentType::Tag))->getTag();
	if (tag == Tag::EnemyBullet || tag == Tag::PlayerBullet)
	{
		static_cast<HealthComponent*>(t_entity.getComponent(ComponentType::Health))->setHealth(0);
		return false;
	}
	return true;
}

void CollisionSystem::handlePlayerCollision(Entity* t_player)
{
	TransformComponent* rectPosition = static_cast<TransformComponent*>(t_player->getComponent(ComponentType::Transform));
	int radius = static_cast<ColliderCircleComponent*>(t_player->getComponent(ComponentType::ColliderCircle))->getRadius();
	glm::vec2 bounds{ radius, radius };
	Quad quad{ NULL, rectPosition->getPos() - bounds, bounds * 2.0f };

	std::vector<Entity*> entities;
	m_quadTree.retrieve(&entities, quad);

	for (auto& other : entities)
	{
		TagComponent* tag = static_cast<TagComponent*>(other->getComponent(ComponentType::Tag));

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
		case Tag::Goal:
			playerToGoal(t_player, other);
			break;
		default:
			break;
		}
	}
}

void CollisionSystem::handlePlayerBulletCollision(Entity* t_playerBullet)
{
	TransformComponent* rectPosition = static_cast<TransformComponent*>(t_playerBullet->getComponent(ComponentType::Transform));
	int radius = static_cast<ColliderCircleComponent*>(t_playerBullet->getComponent(ComponentType::ColliderCircle))->getRadius();
	glm::vec2 bounds{ radius, radius };
	Quad quad{ NULL, rectPosition->getPos() - bounds, bounds * 2.0f };

	std::vector<Entity*> entities;
	m_quadTree.retrieve(&entities, quad);

	for (auto& other : entities)
	{
		TagComponent* tag = static_cast<TagComponent*>(other->getComponent(ComponentType::Tag));

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
	TransformComponent* rectPosition = static_cast<TransformComponent*>(t_enemy->getComponent(ComponentType::Transform));
	int radius = static_cast<ColliderCircleComponent*>(t_enemy->getComponent(ComponentType::ColliderCircle))->getRadius();
	glm::vec2 bounds{ radius, radius };
	Quad quad{ NULL, rectPosition->getPos() - bounds, bounds * 2.0f };

	std::vector<Entity*> entities;
	m_quadTree.retrieve(&entities, quad);

	for (auto& other : entities)
	{
		TagComponent* tag = static_cast<TagComponent*>(other->getComponent(ComponentType::Tag));

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
	if (t_player2->getComponent(ComponentType::ColliderCircle) && circleToCircleCollision(t_player1, t_player2))
	{
		int player1Radius = static_cast<ColliderCircleComponent*>(t_player1->getComponent(ComponentType::ColliderCircle))->getRadius();
		TransformComponent* player1Position = static_cast<TransformComponent*>(t_player1->getComponent(ComponentType::Transform));

		int player2Radius = static_cast<ColliderCircleComponent*>(t_player2->getComponent(ComponentType::ColliderCircle))->getRadius();
		TransformComponent* player2Position = static_cast<TransformComponent*>(t_player2->getComponent(ComponentType::Transform));

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
	if (t_enemy->getComponent(ComponentType::ColliderCircle) && circleToCircleCollision(t_player, t_enemy))
	{
		int playerRadius = static_cast<ColliderCircleComponent*>(t_player->getComponent(ComponentType::ColliderCircle))->getRadius();
		TransformComponent* playerPosition = static_cast<TransformComponent*>(t_player->getComponent(ComponentType::Transform));
		ForceComponent* playerForce = static_cast<ForceComponent*>(t_player->getComponent(ComponentType::Force));
		HealthComponent* playerHealth = static_cast<HealthComponent*>(t_player->getComponent(ComponentType::Health));

		int enemyRadius = static_cast<ColliderCircleComponent*>(t_enemy->getComponent(ComponentType::ColliderCircle))->getRadius();
		TransformComponent* enemyPosition = static_cast<TransformComponent*>(t_enemy->getComponent(ComponentType::Transform));
		ForceComponent* enemyForce = static_cast<ForceComponent*>(t_enemy->getComponent(ComponentType::Force));

		glm::vec2 distanceBetween = glm::normalize(playerPosition->getPos() - enemyPosition->getPos()) * PLAYER_TO_ENEMY_REFECTION_SCALER;

		playerHealth->reduceHealth(1);
		if (playerHealth->isAlive())
		{
			std::string filePath = "playerHurt" + std::to_string(glm::linearRand(1, 3)) + ".wav";
			AudioManager::Instance()->PlaySfx(filePath);
			playerForce->addForce(distanceBetween);
			enemyForce->addForce(-distanceBetween);
		}
		else
		{
			std::string filePath = "gameover" + std::to_string(glm::linearRand(1, 3)) + ".wav";
			AudioManager::Instance()->PlaySfx(filePath);
		}
	}
}

void CollisionSystem::playerToEnemyBullet(Entity* t_player, Entity* t_enemyBullet)
{
	//TODO
}

void CollisionSystem::playerToWall(Entity* t_player, Entity* t_wall)
{
	if (t_wall->getComponent(ComponentType::ColliderAABB) && circleToAABBCollision(t_player, t_wall))
	{
		int playerRadius = static_cast<ColliderCircleComponent*>(t_player->getComponent(ComponentType::ColliderCircle))->getRadius();
		TransformComponent* playerPosition = static_cast<TransformComponent*>(t_player->getComponent(ComponentType::Transform));

		int wallWidth = static_cast<ColliderAABBComponent*>(t_wall->getComponent(ComponentType::ColliderAABB))->getBounds().x;
		TransformComponent* wallPosition = static_cast<TransformComponent*>(t_wall->getComponent(ComponentType::Transform));

		glm::vec2 distanceBetween = playerPosition->getPos() - glm::vec2(playerRadius, playerRadius) - wallPosition->getPos();

		if (distanceBetween.x < playerRadius * 2 + wallWidth && (std::abs(distanceBetween.x) > std::abs(distanceBetween.y)))
		{
			if (distanceBetween.x < 0)
			{
				playerPosition->setPos(-(playerRadius)+wallPosition->getPos().x, playerPosition->getPos().y);
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
	}
}

void CollisionSystem::playerToGoal(Entity* t_player, Entity* t_goal)
{
	if (t_player->getComponent(ComponentType::ColliderCircle) && circleToCircleCollision(t_player, t_goal))
	{
		m_eventManager.emitEvent<ChangeScreen>(ChangeScreen{ MenuStates::MainMenu });
	}
}

void CollisionSystem::playerBulletToEnemy(Entity* t_playerBullet, Entity* t_enemy)
{
	if (t_enemy->getComponent(ComponentType::ColliderCircle) && circleToCircleCollision(t_playerBullet, t_enemy))
	{
		static_cast<HealthComponent*>(t_playerBullet->getComponent(ComponentType::Health))->setHealth(0); //kill the bullet
		static_cast<HealthComponent*>(t_enemy->getComponent(ComponentType::Health))->reduceHealth(1);
	}
}

void CollisionSystem::playerBulletToWall(Entity* t_playerBullet, Entity* t_wall)
{
	if (t_wall->getComponent(ComponentType::ColliderAABB) && circleToAABBCollision(t_playerBullet, t_wall))
	{
		static_cast<HealthComponent*>(t_playerBullet->getComponent(ComponentType::Health))->setHealth(0); //kill the bullet
		static_cast<HealthComponent*>(t_wall->getComponent(ComponentType::Health))->reduceHealth(1);
	}
}

void CollisionSystem::enemyToEnemy(Entity* t_enemy1, Entity* t_enemy2)
{
	if (t_enemy2->getComponent(ComponentType::ColliderCircle) && circleToCircleCollision(t_enemy1, t_enemy2))
	{
		int enemy1Radius = static_cast<ColliderCircleComponent*>(t_enemy1->getComponent(ComponentType::ColliderCircle))->getRadius();
		TransformComponent* enemy1Position = static_cast<TransformComponent*>(t_enemy1->getComponent(ComponentType::Transform));

		int player2Radius = static_cast<ColliderCircleComponent*>(t_enemy2->getComponent(ComponentType::ColliderCircle))->getRadius();
		TransformComponent* enemy2Position = static_cast<TransformComponent*>(t_enemy2->getComponent(ComponentType::Transform));

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
	if (t_wall->getComponent(ComponentType::ColliderAABB) && circleToAABBCollision(t_enemy, t_wall))
	{
		if (t_wall->getComponent(ComponentType::ColliderAABB) && circleToAABBCollision(t_enemy, t_wall))
		{
			int playerRadius = static_cast<ColliderCircleComponent*>(t_enemy->getComponent(ComponentType::ColliderCircle))->getRadius();
			TransformComponent* playerPosition = static_cast<TransformComponent*>(t_enemy->getComponent(ComponentType::Transform));

			int wallWidth = static_cast<ColliderAABBComponent*>(t_wall->getComponent(ComponentType::ColliderAABB))->getBounds().x;
			TransformComponent* wallPosition = static_cast<TransformComponent*>(t_wall->getComponent(ComponentType::Transform));

			glm::vec2 distanceBetween = playerPosition->getPos() - glm::vec2(playerRadius, playerRadius) - wallPosition->getPos();

			if (distanceBetween.x < playerRadius * 2 + wallWidth && (std::abs(distanceBetween.x) > std::abs(distanceBetween.y)))
			{
				if (distanceBetween.x < 0)
				{
					playerPosition->setPos(-(playerRadius)+wallPosition->getPos().x, playerPosition->getPos().y);
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
		}
	}
}

void CollisionSystem::enemyBulletToWall(Entity* t_enemyBullet, Entity* t_wall)
{
	if (t_wall->getComponent(ComponentType::ColliderAABB) && circleToAABBCollision(t_enemyBullet, t_wall))
	{
		static_cast<HealthComponent*>(t_enemyBullet->getComponent(ComponentType::Health))->setHealth(0); //kill the bullet
	}
}
