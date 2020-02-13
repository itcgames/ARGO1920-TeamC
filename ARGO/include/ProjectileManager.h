#pragma once
#include "Entity.h"
#include "TransformComponent.h"
#include "ForceComponent.h"
#include "ColliderCircleComponent.h"
#include "ColourComponent.h"
#include "HealthComponent.h"

struct createBulletEvent
{
	glm::vec2 position;
	glm::vec2 force;
	int type;
};

struct Bullet
{
	Entity entity;
	int type;
};

class ProjectileManager
{
public:
	ProjectileManager();
private:
	void createPlayerBullet(createBulletEvent event);
	void createEnemyBullet(createBulletEvent event);

	static const int BULLET_POOL_SIZE = 20;
	static const int BULLET_RADIUS = 4;
	Bullet m_playerBullets[BULLET_POOL_SIZE];
	Bullet m_enemyBullets[BULLET_POOL_SIZE];
	int m_nextPlayerBullet;
	int m_nextEnemyBullet;
};

