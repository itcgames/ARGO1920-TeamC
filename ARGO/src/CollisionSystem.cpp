#include "stdafx.h"
#include "CollisionSystem.h"

CollisionSystem::CollisionSystem(EventManager& t_eventManager) :
	m_eventManager(t_eventManager),
	m_quadTree(0, glm::vec2(0, 0), glm::vec2(Utilities::LEVEL_TILE_WIDTH* Utilities::TILE_SIZE, Utilities::LEVEL_TILE_HEIGHT* Utilities::TILE_SIZE))
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
		case Tag::GlowStick:
			handleGlowStickCollision(m_circleColliderBuffer[i]);
			break;
		case Tag::Grenade:
			handleGrenadeCollision(m_circleColliderBuffer[i]);
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

bool CollisionSystem::circleToCircleCollision(glm::vec2 t_pos1, float t_radius1, glm::vec2 t_pos2, float t_radius2)
{
	glm::vec2 distanceBetween = t_pos1 - t_pos2;
	return sqrt(distanceBetween.x * distanceBetween.x + distanceBetween.y * distanceBetween.y) < t_radius1 + t_radius2;
}

bool CollisionSystem::circleToAABBCollision(glm::vec2 t_pos1, float t_radius, glm::vec2 t_pos2, glm::vec2 t_bounds)
{
	int deltaX = t_pos1.x - std::max(t_pos2.x, std::min(t_pos1.x, t_pos2.x + t_bounds.x));
	int deltaY = t_pos1.y - std::max(t_pos2.y, std::min(t_pos1.y, t_pos2.y + t_bounds.y));
	return (deltaX * deltaX + deltaY * deltaY) < (t_radius * t_radius);
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
		case Tag::PickUp:
			playerToPickUp(t_player, other);
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

void CollisionSystem::handleGrenadeCollision(Entity* t_grenade)
{
	TransformComponent* rectPosition = static_cast<TransformComponent*>(t_grenade->getComponent(ComponentType::Transform));
	int radius = static_cast<ColliderCircleComponent*>(t_grenade->getComponent(ComponentType::ColliderCircle))->getRadius();
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
			grenadeToEnemy(t_grenade, other);
			break;
		case Tag::Tile:
			greandeToWall(t_grenade, other);
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

void CollisionSystem::handleGlowStickCollision(Entity* t_glowStick)
{
	TransformComponent* rectPosition = static_cast<TransformComponent*>(t_glowStick->getComponent(ComponentType::Transform));
	int radius = static_cast<ColliderCircleComponent*>(t_glowStick->getComponent(ComponentType::ColliderCircle))->getRadius();
	glm::vec2 bounds{ radius, radius };
	Quad quad{ NULL, rectPosition->getPos() - bounds, bounds * 2.0f };

	std::vector<Entity*> entities;
	m_quadTree.retrieve(&entities, quad);

	for (auto& other : entities)
	{
		TagComponent* tag = static_cast<TagComponent*>(other->getComponent(ComponentType::Tag));

		switch (tag->getTag())
		{
		case Tag::Tile:
			glowStickToWall(t_glowStick, other);
			break;
		default:
			break;
		}
	}
}

void CollisionSystem::createExplosion(glm::vec2 t_position)
{
	glm::vec2 bounds{ EXPLOSION_RADIUS, EXPLOSION_RADIUS };
	Quad quad{ NULL, t_position - bounds, bounds * 2.0f };
	m_eventManager.emitEvent(Explosion{ t_position });
	std::string filePath = "Explosion_" + std::to_string(glm::linearRand(1, 2)) + ".wav";
	AudioManager::Instance()->PlaySfx(Utilities::GUN_FIRE_PATH + filePath);

	std::vector<Entity*> entities;
	m_quadTree.retrieve(&entities, quad); 
	for (auto& other : entities)
	{
		TagComponent* tag = static_cast<TagComponent*>(other->getComponent(ComponentType::Tag));
		TransformComponent* transform = static_cast<TransformComponent*>(other->getComponent(ComponentType::Transform));

		switch (tag->getTag())
		{
		case Tag::Enemy:
			if (other->getComponent(ComponentType::ColliderCircle) && circleToCircleCollision(t_position, EXPLOSION_RADIUS, transform->getPos(), static_cast<ColliderCircleComponent*>(other->getComponent(ComponentType::ColliderCircle))->getRadius()))
			{
				static_cast<HealthComponent*>(other->getComponent(ComponentType::Health))->setHealth(0);
				m_eventManager.emitEvent(EnemyKilled{ other });
			}
			break;
		case Tag::Tile:
			if (other->getComponent(ComponentType::ColliderAABB) && circleToAABBCollision(t_position, EXPLOSION_RADIUS, transform->getPos(), static_cast<ColliderAABBComponent*>(other->getComponent(ComponentType::ColliderAABB))->getBounds()))
			{
				static_cast<HealthComponent*>(other->getComponent(ComponentType::Health))->setHealth(0);
			}
			break;
		default:
			break;
		}
	}
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
			distanceBetween = distanceBetween * seperation * SEPERATION_SCALER;
		}
		else
		{
			distanceBetween = glm::vec2(SEPERATION_SCALER, SEPERATION_SCALER);
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
		InputComponent* playerInput = static_cast<InputComponent*>(t_player->getComponent(ComponentType::Input));
		if (playerInput)
		{
			playerInput->getController().activateRumble(RumbleStrength::Strong, RumbleLength::Short);
		}

		int enemyRadius = static_cast<ColliderCircleComponent*>(t_enemy->getComponent(ComponentType::ColliderCircle))->getRadius();
		TransformComponent* enemyPosition = static_cast<TransformComponent*>(t_enemy->getComponent(ComponentType::Transform));
		ForceComponent* enemyForce = static_cast<ForceComponent*>(t_enemy->getComponent(ComponentType::Force));

		glm::vec2 distanceBetween = glm::normalize(playerPosition->getPos() - enemyPosition->getPos()) * PLAYER_TO_ENEMY_REFECTION_SCALER;

		//if took damage and is alive
		if (playerHealth->reduceHealth(1) && playerHealth->isAlive())
		{
			//play hurt sound and bounce back
			std::string filePath = "playerHurt" + std::to_string(glm::linearRand(1, 3)) + ".wav";
			AudioManager::Instance()->PlaySfx(filePath);
			playerForce->addForce(distanceBetween);
			enemyForce->addForce(-distanceBetween);
		}
		//else if in iframes but is alive
		else if (playerHealth->isAlive())
		{
			//bounce back
			playerForce->addForce(distanceBetween);
			enemyForce->addForce(-distanceBetween);
		}
		//otherwise player died
		else
		{
			//so play death sound
			m_eventManager.emitEvent(PlayerKilled{ t_player });
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
		ForceComponent* force = static_cast<ForceComponent*>(t_player->getComponent(ComponentType::Force));

		int wallWidth = static_cast<ColliderAABBComponent*>(t_wall->getComponent(ComponentType::ColliderAABB))->getBounds().x;
		TransformComponent* wallPosition = static_cast<TransformComponent*>(t_wall->getComponent(ComponentType::Transform));

		glm::vec2 previousPos = playerPosition->getPos() - force->getForce();
		glm::vec2 previousDistanceBetween = previousPos - (wallPosition->getPos() + glm::vec2(wallWidth / 2, wallWidth / 2));

		if (previousDistanceBetween.x < playerRadius * 2 + wallWidth && (std::abs(previousDistanceBetween.x) > std::abs(previousDistanceBetween.y)))
		{
			if (previousDistanceBetween.x < 0)
			{
				playerPosition->setPos(-(playerRadius)+wallPosition->getPos().x, playerPosition->getPos().y);
			}
			else
			{
				playerPosition->setPos((playerRadius + wallWidth) + wallPosition->getPos().x, playerPosition->getPos().y);
			}
		}
		if (previousDistanceBetween.y < playerRadius * 2 + wallWidth && (std::abs(previousDistanceBetween.x) < std::abs(previousDistanceBetween.y)))
		{
			if (previousDistanceBetween.y < 0)
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

void CollisionSystem::playerToPickUp(Entity* t_player, Entity* t_pickUp)
{
	if (t_pickUp->getComponent(ComponentType::ColliderCircle) && circleToCircleCollision(t_player, t_pickUp))
	{
		HealthComponent* healthComp = static_cast<HealthComponent*>(t_pickUp->getComponent(ComponentType::Health));
		if (healthComp->isAlive())
		{
			m_eventManager.emitEvent(PickupGrabbed{ t_pickUp });
			PickUpComponent* pickUpComp = static_cast<PickUpComponent*>(t_pickUp->getComponent(ComponentType::PickUp));
			WeaponComponent* weaponComp = static_cast<WeaponComponent*>(t_player->getComponent(ComponentType::Weapon));

			switch (pickUpComp->getPickupType())
			{
			case PickupType::MachineGun:
				weaponComp->fillAmmo(Weapon::MachineGun);
				break;
			case PickupType::Grenade:
				weaponComp->fillAmmo(Weapon::GrenadeLauncher);
				break;
			case PickupType::Shotgun:
				weaponComp->fillAmmo(Weapon::Shotgun);
				break;
			case PickupType::Health:
				//Health Pickup
				HealthComponent * playerHealthComp = static_cast<HealthComponent*>(t_player->getComponent(ComponentType::Health));
				int healthToAdd = playerHealthComp->getMaxHealth() * pickUpComp->getHealthChange();
				if (healthToAdd <= 0)
				{
					healthToAdd = 1;
				}
				playerHealthComp->addHealth(healthToAdd);
				break;
			}


		}
	}

}

void CollisionSystem::playerToGoal(Entity* t_player, Entity* t_goal)
{
	if (t_player->getComponent(ComponentType::ColliderCircle) && circleToCircleCollision(t_player, t_goal))
	{
		m_eventManager.emitEvent<UpdateAchievement>(UpdateAchievement{ 0, 1 });
		m_eventManager.emitEvent<ChangeScreen>(ChangeScreen{ MenuStates::MainMenu });
	}
}

void CollisionSystem::playerBulletToEnemy(Entity* t_playerBullet, Entity* t_enemy)
{
	if (t_enemy->getComponent(ComponentType::ColliderCircle) && circleToCircleCollision(t_playerBullet, t_enemy))
	{
		static_cast<HealthComponent*>(t_playerBullet->getComponent(ComponentType::Health))->setHealth(0); //kill the bullet
		static_cast<HealthComponent*>(t_enemy->getComponent(ComponentType::Health))->reduceHealth(1);
		if (!static_cast<HealthComponent*>(t_enemy->getComponent(ComponentType::Health))->isAlive())
		{
			m_eventManager.emitEvent<UpdateAchievement>(UpdateAchievement{ 1,0 });
			m_eventManager.emitEvent(EnemyKilled{ t_enemy });
			std::string filePath = "BugDeath_" + std::to_string(glm::linearRand(1, 10)) + ".wav";
			AudioManager::Instance()->PlaySfx(Utilities::BUG_DEATH_PATH + filePath);
		}
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
			distanceBetween = distanceBetween * seperation * SEPERATION_SCALER;
		}
		else
		{
			distanceBetween = glm::vec2(SEPERATION_SCALER, SEPERATION_SCALER);
		}
		enemy1Position->addPos(distanceBetween);
		enemy2Position->addPos(-distanceBetween);
	}
}

void CollisionSystem::enemyToWall(Entity* t_enemy, Entity* t_wall)
{
	if (t_wall->getComponent(ComponentType::ColliderAABB) && circleToAABBCollision(t_enemy, t_wall))
	{
		int enemyRadius = static_cast<ColliderCircleComponent*>(t_enemy->getComponent(ComponentType::ColliderCircle))->getRadius();
		TransformComponent* enemyPosition = static_cast<TransformComponent*>(t_enemy->getComponent(ComponentType::Transform));
		ForceComponent* force = static_cast<ForceComponent*>(t_enemy->getComponent(ComponentType::Force));

		int wallWidth = static_cast<ColliderAABBComponent*>(t_wall->getComponent(ComponentType::ColliderAABB))->getBounds().x;
		TransformComponent* wallPosition = static_cast<TransformComponent*>(t_wall->getComponent(ComponentType::Transform));

		glm::vec2 previousPos = enemyPosition->getPos() - force->getForce();
		glm::vec2 previousDistanceBetween = previousPos - (wallPosition->getPos() + glm::vec2(wallWidth / 2, wallWidth / 2));

		if (previousDistanceBetween.x < enemyRadius * 2 + wallWidth && (std::abs(previousDistanceBetween.x) > std::abs(previousDistanceBetween.y)))
		{
			if (previousDistanceBetween.x < 0)
			{
				enemyPosition->setPos(-(enemyRadius)+wallPosition->getPos().x, enemyPosition->getPos().y);
			}
			else
			{
				enemyPosition->setPos((enemyRadius + wallWidth) + wallPosition->getPos().x, enemyPosition->getPos().y);
			}
		}
		if (previousDistanceBetween.y < enemyRadius * 2 + wallWidth && (std::abs(previousDistanceBetween.x) < std::abs(previousDistanceBetween.y)))
		{
			if (previousDistanceBetween.y < 0)
			{
				enemyPosition->setPos(enemyPosition->getPos().x, -(enemyRadius)+wallPosition->getPos().y);
			}
			else
			{
				enemyPosition->setPos(enemyPosition->getPos().x, (enemyRadius + wallWidth) + wallPosition->getPos().y);
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

void CollisionSystem::glowStickToWall(Entity* t_glowstick, Entity* t_wall)
{
	if (t_wall->getComponent(ComponentType::ColliderAABB) && circleToAABBCollision(t_glowstick, t_wall))
	{
		int glowStickRadius = static_cast<ColliderCircleComponent*>(t_glowstick->getComponent(ComponentType::ColliderCircle))->getRadius();
		TransformComponent* glowStickPosition = static_cast<TransformComponent*>(t_glowstick->getComponent(ComponentType::Transform));
		ForceComponent* glowStickForce = static_cast<ForceComponent*>(t_glowstick->getComponent(ComponentType::Force));

		int wallWidth = static_cast<ColliderAABBComponent*>(t_wall->getComponent(ComponentType::ColliderAABB))->getBounds().x;
		TransformComponent* wallPosition = static_cast<TransformComponent*>(t_wall->getComponent(ComponentType::Transform));

		glm::vec2 previousPos = glowStickPosition->getPos() - glowStickForce->getForce();
		glm::vec2 previousDistanceBetween = previousPos - (wallPosition->getPos() + glm::vec2(wallWidth / 2, wallWidth / 2));

		if (previousDistanceBetween.x < glowStickRadius * 2 + wallWidth && (std::abs(previousDistanceBetween.x) > std::abs(previousDistanceBetween.y)))
		{
			if (previousDistanceBetween.x < 0)
			{
				glowStickPosition->setPos(-(glowStickRadius)+wallPosition->getPos().x, glowStickPosition->getPos().y);
			}
			else
			{
				glowStickPosition->setPos((glowStickRadius + wallWidth) + wallPosition->getPos().x, glowStickPosition->getPos().y);
			}
		}
		if (previousDistanceBetween.y < glowStickRadius * 2 + wallWidth && (std::abs(previousDistanceBetween.x) < std::abs(previousDistanceBetween.y)))
		{
			if (previousDistanceBetween.y < 0)
			{
				glowStickPosition->setPos(glowStickPosition->getPos().x, -(glowStickRadius)+wallPosition->getPos().y);
			}
			else
			{
				glowStickPosition->setPos(glowStickPosition->getPos().x, (glowStickRadius + wallWidth) + wallPosition->getPos().y);
			}
		}
	}
}

void CollisionSystem::grenadeToEnemy(Entity* t_grenade, Entity* t_enemy)
{
	if (t_enemy->getComponent(ComponentType::ColliderCircle) && circleToCircleCollision(t_grenade, t_enemy))
	{
		static_cast<HealthComponent*>(t_grenade->getComponent(ComponentType::Health))->setHealth(0); //kill the bullet
		createExplosion(static_cast<TransformComponent*>(t_grenade->getComponent(ComponentType::Transform))->getPos());
	}
}

void CollisionSystem::greandeToWall(Entity* t_grenade, Entity* t_wall)
{
	if (t_wall->getComponent(ComponentType::ColliderAABB) && circleToAABBCollision(t_grenade, t_wall))
	{
		createExplosion(static_cast<TransformComponent*>(t_grenade->getComponent(ComponentType::Transform))->getPos());
		static_cast<HealthComponent*>(t_grenade->getComponent(ComponentType::Health))->setHealth(0); //kill the bullet
	}
}
