#pragma once
#include "Entity.h"
#include "BaseSystem.h"
#include "RenderSystem.h"
#include "TransformComponent.h"
#include "ForceComponent.h"
#include "ColliderCircleComponent.h"
#include "ColourComponent.h"
#include "HealthComponent.h"
#include "TimerComponent.h"

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
	void createPlayerBullet(createBulletEvent t_event);
	void createEnemyBullet(createBulletEvent t_event);
	void update(BaseSystem* t_system);
	void render(SDL_Renderer* t_renderer, RenderSystem* t_system);
private:
	static const int BULLET_POOL_SIZE = 20;
	static const int BULLET_RADIUS = 4;
	static const int BULLET_LIFETIME = 30;
	Bullet m_playerBullets[BULLET_POOL_SIZE];
	Bullet m_enemyBullets[BULLET_POOL_SIZE];
	int m_nextPlayerBullet;
	int m_nextEnemyBullet;
};

