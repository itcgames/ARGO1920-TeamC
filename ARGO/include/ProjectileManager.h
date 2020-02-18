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
#include "TagComponent.h"
#include "FireRateComponent.h"
#include "EventManager.h"
#include <AudioManager.h>

struct Bullet
{
	Entity entity;
	int type;
};

class ProjectileManager
{
public:
	ProjectileManager(EventManager& t_eventManager, glm::vec2& t_focusPoint);
	void init();
	void createPlayerBullet(const createBulletEvent& t_event);
	void createEnemyBullet(const createBulletEvent& t_event);
	void update(BaseSystem* t_system);
	void tick();
	void render(SDL_Renderer* t_renderer, RenderSystem* t_system);
private:
	static const int BULLET_POOL_SIZE = 160;
	static const int BULLET_RADIUS = 4;
	static const int BULLET_LIFETIME = 30;
	Bullet m_playerBullets[BULLET_POOL_SIZE];
	Bullet m_enemyBullets[BULLET_POOL_SIZE];
	int m_nextPlayerBullet;
	int m_nextEnemyBullet;
	AudioManager* m_audioMgr;
	glm::vec2& m_focusPoint;

};

