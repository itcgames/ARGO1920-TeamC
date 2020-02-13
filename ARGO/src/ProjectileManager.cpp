#include "stdafx.h"
#include "ProjectileManager.h"

ProjectileManager::ProjectileManager() :
	m_nextEnemyBullet(0),
	m_nextPlayerBullet(0)
{
	for (auto& bullet : m_playerBullets)
	{
		bullet.entity.addComponent(new TransformComponent());
		bullet.entity.addComponent(new ColourComponent(glm::linearRand(0, 255), glm::linearRand(0, 255), glm::linearRand(0, 255), 255));
		bullet.entity.addComponent(new ForceComponent());
		bullet.entity.addComponent(new HealthComponent(1, 0));
		bullet.entity.addComponent(new ColliderCircleComponent(BULLET_RADIUS));
	}
	for (auto& bullet : m_enemyBullets)
	{
		bullet.entity.addComponent(new TransformComponent());
		bullet.entity.addComponent(new ColourComponent(glm::linearRand(0, 255), glm::linearRand(0, 255), glm::linearRand(0, 255), 255));
		bullet.entity.addComponent(new ForceComponent());
		bullet.entity.addComponent(new HealthComponent(1, 0));
		bullet.entity.addComponent(new ColliderCircleComponent(BULLET_RADIUS));
	}
}

void ProjectileManager::createPlayerBullet(createBulletEvent event)
{
	static_cast<TransformComponent*>(m_playerBullets[m_nextPlayerBullet].entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->setPos(event.position);
	static_cast<ForceComponent*>(m_playerBullets[m_nextPlayerBullet].entity.getAllComps().at(COMPONENT_ID::FORCE_ID))->setForce(event.force);
	static_cast<HealthComponent*>(m_playerBullets[m_nextPlayerBullet].entity.getAllComps().at(COMPONENT_ID::HEALTH_ID))->setHealth(1);
	m_playerBullets[m_nextPlayerBullet].type = event.type;

	m_nextPlayerBullet++;
	if (m_nextPlayerBullet >= BULLET_POOL_SIZE)
	{
		m_nextPlayerBullet = 0;
	}
}

void ProjectileManager::createEnemyBullet(createBulletEvent event)
{
	static_cast<TransformComponent*>(m_enemyBullets[m_nextEnemyBullet].entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->setPos(event.position);
	static_cast<ForceComponent*>(m_enemyBullets[m_nextEnemyBullet].entity.getAllComps().at(COMPONENT_ID::FORCE_ID))->setForce(event.force);
	static_cast<HealthComponent*>(m_enemyBullets[m_nextEnemyBullet].entity.getAllComps().at(COMPONENT_ID::HEALTH_ID))->setHealth(1);
	m_enemyBullets[m_nextEnemyBullet].type = event.type;

	m_nextEnemyBullet++;
	if (m_nextEnemyBullet >= BULLET_POOL_SIZE)
	{
		m_nextEnemyBullet = 0;
	}
}
