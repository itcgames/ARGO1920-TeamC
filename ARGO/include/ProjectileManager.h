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
#include "WeaponComponent.h"
#include "EventManager.h"
#include "PhysicsSystem.h"
#include "CollisionSystem.h"
#include <AudioManager.h>

struct Bullet
{
	Entity entity;
	Weapon type;
};

class ProjectileManager
{
public:
	ProjectileManager(SDL_Renderer* t_renderer, EventManager& t_eventManager, glm::vec2& t_focusPoint, PhysicsSystem& t_physicsSystem, CollisionSystem& t_collisionSystem);
	void init();
	void createPlayerBullet(const CreateBulletEvent& t_event, Weapon t_weapon);
	void createGlowStick(const CreateGlowStickEvent& t_event);
	void createEnemyBullet(const CreateBulletEvent& t_event);
	void update(float t_dt);
	void updateBullet(Bullet& t_bullet, float t_dt);
	void updateGlowStick(Entity& t_glowStick, float t_dt);
	void render(SDL_Renderer* t_renderer, RenderSystem* t_system);
	void reset();
	Entity(&getGlowsticks())[Utilities::GLOWSTICK_POOL_SIZE];
private:
	void tick(float t_dt);

	static const int BULLET_POOL_SIZE = 160;
	static const int BULLET_RADIUS = 6;
	static const int GLOWSTICK_RADIUS = 16;
	static const int BULLET_LIFETIME = 90;
	static const int GLOWSTICK_LIFETIME = 300;
	const float PLAYER_BULLET_SPEED = 16.0f;
	const float PLAYER_GRENADE_SPEED = 16.0f;
	const float GLOWSTICK_SPEED = 64;

	Bullet m_playerBullets[BULLET_POOL_SIZE];
	Entity m_glowsticks[Utilities::GLOWSTICK_POOL_SIZE];
	Bullet m_enemyBullets[BULLET_POOL_SIZE];

	int m_nextPlayerBullet;
	int m_nextEnemyBullet;
	int m_nextGlowStick;

	AudioManager* m_audioMgr;
	glm::vec2& m_focusPoint;

	PhysicsSystem& m_physicsSystem;
	CollisionSystem& m_collisionSystem;

	SDL_Renderer* m_renderer;
};

