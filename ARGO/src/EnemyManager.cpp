#include "stdafx.h"
#include "EnemyManager.h"

EnemyManager::EnemyManager(SDL_Renderer* t_renderer, float t_initialSpawnDelay, EventManager& t_eventManager, PhysicsSystem& t_physicsSystem, CollisionSystem& t_collisionSystem, HealthSystem& t_healthSystem, AiSystem& t_aiSystem, RenderSystem& t_renderSystem, LevelManager& t_levelManager) :
	m_nextEnemy(0),
	m_renderer(t_renderer),
	m_spawnEnemyTimer(t_initialSpawnDelay),
	m_physicsSystem(t_physicsSystem),
	m_collisionSystem(t_collisionSystem),
	m_healthSystem(t_healthSystem),
	m_aiSytem(t_aiSystem),
	m_renderSystem(t_renderSystem),
	m_levelManager(t_levelManager),
	m_difficultyLevel(0),
	m_difficultyTimer(0)
{
}

void EnemyManager::init()
{
	m_audioMgr = AudioManager::Instance();
	EnemyFactory* factory = new EnemyFactory(m_renderer);
	for (auto& enemy : m_enemies)
	{
		factory->createEnemy(1, enemy);
	}
}

void EnemyManager::update(float t_dt)
{
	m_difficultyTimer += t_dt;
	if (m_difficultyTimer >= DIFFICULTY_INCREASE_RATE)
	{
		m_difficultyTimer = 0;
		m_difficultyLevel++;
		std::cout << m_difficultyLevel << std::endl;
	}
	spawnGroup(t_dt);
	for (auto& enemy : m_enemies)
	{
		HealthComponent* hpComp = static_cast<HealthComponent*>(enemy.getComponent(ComponentType::Health));
		if (hpComp && hpComp->isAlive())
		{
			m_healthSystem.update(enemy, t_dt);
			m_physicsSystem.update(enemy, t_dt);
			m_collisionSystem.update(enemy);
			m_aiSytem.update(enemy);
		}
	}
}

void EnemyManager::spawnGroup(float t_dt)
{
	m_spawnEnemyTimer -= t_dt;
	if (m_spawnEnemyTimer <= 0)
	{
		float nextSpawn = SPAWN_ENEMY_RATE;
		if (m_difficultyLevel > 4)
		{
			nextSpawn -= (m_difficultyLevel + 4) * 60;
			if (nextSpawn < 60) nextSpawn = 60;
		}

		m_spawnEnemyTimer = nextSpawn;
		Entity* spawnTile = nullptr;
		int spawnAttempts = 0;
		const int maxAttempts = 20;
		while (spawnTile == nullptr && spawnAttempts < maxAttempts)
		{
			int x = glm::linearRand(-5, 5);
			int y = glm::linearRand(-5, 5);
			x += x > 0 ? 16 : -16;
			y += y > 0 ? 10 : -10;
			spawnTile = m_levelManager.findAtPosition(glm::vec2(x * Utilities::TILE_SIZE, y * Utilities::TILE_SIZE) + m_renderSystem.getFocus());
			if (spawnTile)
			{
				ColliderAABBComponent* aabbComp = static_cast<ColliderAABBComponent*>(spawnTile->getComponent(ComponentType::ColliderAABB));
				if (aabbComp)
				{
					spawnTile = nullptr;
				}
			}
			spawnAttempts++;
		}

		if (spawnTile)
		{
			Neighbours* neighbours = static_cast<TileComponent*>(spawnTile->getComponent(ComponentType::Tile))->getNeighbours();
			createEnemyAtTile(spawnTile);
			createEnemyAtTile(neighbours->top);
			createEnemyAtTile(neighbours->left);
			if (m_difficultyLevel > 1)
			{
				createEnemyAtTile(neighbours->right);
				createEnemyAtTile(neighbours->bottom);
			}
			if (m_difficultyLevel > 2)
			{
				createEnemyAtTile(neighbours->topLeft);
				createEnemyAtTile(neighbours->topRight);
			}
			if (m_difficultyLevel > 3)
			{
				createEnemyAtTile(neighbours->bottomLeft);
				createEnemyAtTile(neighbours->bottomRight);
			}
		}
	}
}

void EnemyManager::createEnemyAtTile(Entity* t_tile)
{
	if (t_tile)
	{
		TransformComponent* tileTransform = static_cast<TransformComponent*>(t_tile->getComponent(ComponentType::Transform));
		HealthComponent* healthComp = static_cast<HealthComponent*> (m_enemies[m_nextEnemy].getComponent(ComponentType::Health));
		TransformComponent* transformComp = static_cast<TransformComponent*> (m_enemies[m_nextEnemy].getComponent(ComponentType::Transform));
		ForceComponent* forceComp = static_cast<ForceComponent*> (m_enemies[m_nextEnemy].getComponent(ComponentType::Force));
		AiComponent* aiComp = static_cast<AiComponent*> (m_enemies[m_nextEnemy].getComponent(ComponentType::Ai));
		ColliderAABBComponent* aabbComp = static_cast<ColliderAABBComponent*>(m_levelManager.findAtPosition(tileTransform->getPos())->getComponent(ComponentType::ColliderAABB));

		if (healthComp && transformComp && forceComp && aiComp && tileTransform && !aabbComp)
		{
			healthComp->resetHealth();
			transformComp->setPos(tileTransform->getPos() + glm::vec2(Utilities::ENEMY_RADIUS, Utilities::ENEMY_RADIUS));
			transformComp->setRotation(0);
			forceComp->setForce(glm::vec2(0, 0));
			aiComp->setType(AITypes::eMelee);
			aiComp->setState(AIStates::eWander);

			m_nextEnemy++;
			if (m_nextEnemy >= Utilities::ENEMY_POOL_SIZE)
			{
				m_nextEnemy = 0;
			}
		}
	}
}

Entity(&EnemyManager::getEnemies())[Utilities::ENEMY_POOL_SIZE]
{
	return m_enemies;
}

void EnemyManager::killAll()
{
	for (auto& enemy : m_enemies)
	{
		HealthComponent* hpComp = static_cast<HealthComponent*>(enemy.getComponent(ComponentType::Health));
		if (hpComp)
		{
			hpComp->setHealth(0);
		}
	}
}

void EnemyManager::render(SDL_Renderer* t_renderer)
{
	for (auto& enemy : m_enemies)
	{
		HealthComponent* hpComp = static_cast<HealthComponent*>(enemy.getComponent(ComponentType::Health));
		if (hpComp && hpComp->isAlive())
		{
			m_renderSystem.render(t_renderer, enemy);
		}
	}
}