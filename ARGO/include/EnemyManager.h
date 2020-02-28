#pragma once

#include "Entity.h"
#include "AiSystem.h"
#include "HealthSystem.h"
#include "PhysicsSystem.h"
#include "CollisionSystem.h"
#include "RenderSystem.h"
#include "AudioManager.h"
#include "LevelManager.h"
#include "Utilities.h"
#include "EnemyFactory.h"

class EnemyManager
{
public:
	EnemyManager(SDL_Renderer* t_renderer, float t_initialSpawnDelay, EventManager& t_eventManager, PhysicsSystem& t_physicsSystem, CollisionSystem& t_collisionSystem, HealthSystem& t_healthSystem, AiSystem& t_aiSystem, RenderSystem& t_renderSystem, LevelManager& t_levelManager);
	void init();
	void update(float t_dt);
	void render(SDL_Renderer* t_renderer);
	void spawnGroup(float t_dt);
	void createEnemyAtTile(Entity* t_tile);
	Entity(&getEnemies())[Utilities::ENEMY_POOL_SIZE];
	void killAll();

private:
	static const int ENEMY_BASIC_RADIUS = 24;
	Entity m_enemies[Utilities::ENEMY_POOL_SIZE];
	int m_nextEnemy;
	float m_spawnEnemyTimer;
	int m_difficultyLevel;
	float m_difficultyTimer;
	const float DIFFICULTY_INCREASE_RATE = 900;
	const float SPAWN_ENEMY_RATE = 300; //1 second is 60 ticks
	AudioManager* m_audioMgr;
	LevelManager& m_levelManager;

	PhysicsSystem& m_physicsSystem;
	CollisionSystem& m_collisionSystem;
	HealthSystem& m_healthSystem;
	AiSystem& m_aiSytem;
	RenderSystem& m_renderSystem;

	SDL_Renderer* m_renderer;
};