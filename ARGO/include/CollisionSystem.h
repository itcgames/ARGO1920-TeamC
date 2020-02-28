#pragma once
#include "BaseSystem.h"
#include "ColliderAABBComponent.h"
#include "ColliderCircleComponent.h"
#include "TransformComponent.h"
#include "TagComponent.h"
#include "HealthComponent.h"
#include "ForceComponent.h"
#include "PickUpComponent.h"
#include "InputComponent.h"
#include "WeaponComponent.h"
#include "Utilities.h"
#include "QuadTree.h"
#include "AudioManager.h"

class CollisionSystem : public BaseSystem
{
public:
	CollisionSystem(EventManager& t_eventManager);
	void update(Entity& t_entity);
	void handleCollisions();

private:
	bool circleToCircleCollision(Entity* t_entityCircle1, Entity* t_entityCircle2);
	bool circleToAABBCollision(Entity* t_entityCircle, Entity* t_entityAABB);
	bool circleToCircleCollision(glm::vec2 t_pos1, float t_radius1, glm::vec2 t_pos2, float t_radius2);
	bool circleToAABBCollision(glm::vec2 t_pos1, float t_radius, glm::vec2 t_pos2, glm::vec2 t_bounds);
	bool edgeOfTheWorldToCircle(Entity& t_entity);
	bool killBulletAtEdgeOfWorld(Entity& t_entity);

	void handlePlayerCollision(Entity* t_player);
	void handlePlayerBulletCollision(Entity* t_playerBullet);
	void handleGrenadeCollision(Entity* t_grenade);
	void handleEnemyCollision(Entity* t_enemy);
	void handleEnemyBulletCollision(Entity* t_enemyBullet);
	void handleGlowStickCollision(Entity* t_glowStick);
	void createExplosion(glm::vec2 t_position);

	//collision filters
	void playerToPlayer(Entity* t_player1, Entity* t_player2);
	void playerToEnemy(Entity* t_player, Entity* t_enemy);
	void playerToEnemyBullet(Entity* t_player, Entity* t_enemyBullet);
	void playerToWall(Entity* t_player, Entity* t_wall);
	void playerToPickUp(Entity* t_player, Entity* t_pickUp);
	void playerToGoal(Entity* t_player, Entity* t_goal);

	void playerBulletToEnemy(Entity* t_playerBullet, Entity* t_enemy);
	void playerBulletToWall(Entity* t_playerBullet, Entity* t_wall);

	void enemyToEnemy(Entity* t_enemy1, Entity* t_enemy2);
	void enemyToWall(Entity* t_enemy, Entity* t_wall);

	void enemyBulletToWall(Entity* t_enemyBullet, Entity* t_wall);

	void glowStickToWall(Entity* t_glowstick, Entity* t_wall);

	void grenadeToEnemy(Entity* t_grenade, Entity* t_enemy);
	void greandeToWall(Entity* t_grenade, Entity* t_wall);

	EventManager& m_eventManager;
	const float PLAYER_TO_ENEMY_REFECTION_SCALER = 15.0f;
	QuadTree m_quadTree;
	std::vector<Entity*> m_circleColliderBuffer;

	const float SEPERATION_SCALER = 20;
	const float EXPLOSION_RADIUS = 128;
};

