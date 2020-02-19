#include "stdafx.h"
#include "ProjectileManager.h"

ProjectileManager::ProjectileManager(EventManager& t_eventManager) :
	m_nextEnemyBullet(0),
	m_nextPlayerBullet(0)
{
	t_eventManager.subscribeToEvent<createBulletEvent>(std::bind(&ProjectileManager::createPlayerBullet, this, std::placeholders::_1));

	for (auto& bullet : m_playerBullets)
	{
		bullet.entity.addComponent(new TransformComponent());
		bullet.entity.addComponent(new ColourComponent(glm::linearRand(0, 255), glm::linearRand(0, 255), glm::linearRand(0, 255), 255));
		bullet.entity.addComponent(new ForceComponent(glm::vec2(0,0), false));
		bullet.entity.addComponent(new HealthComponent(1, 0));
		bullet.entity.addComponent(new ColliderCircleComponent(BULLET_RADIUS));
		bullet.entity.addComponent(new TimerComponent(BULLET_LIFETIME));
		bullet.entity.addComponent(new TagComponent(Tag::PlayerBullet));
	}
	for (auto& bullet : m_enemyBullets)
	{
		bullet.entity.addComponent(new TransformComponent());
		bullet.entity.addComponent(new ColourComponent(glm::linearRand(0, 255), glm::linearRand(0, 255), glm::linearRand(0, 255), 255));
		bullet.entity.addComponent(new ForceComponent(glm::vec2(0, 0), false));
		bullet.entity.addComponent(new HealthComponent(1, 0));
		bullet.entity.addComponent(new ColliderCircleComponent(BULLET_RADIUS));
		bullet.entity.addComponent(new TimerComponent(BULLET_LIFETIME));
		bullet.entity.addComponent(new TagComponent(Tag::EnemyBullet));
	}
}

void ProjectileManager::createPlayerBullet(const createBulletEvent& t_event)
{
	FireRateComponent* fireRateComp = static_cast<FireRateComponent*>(t_event.entity.getComponent(ComponentType::FireRate));
	Uint16 currentTick = SDL_GetTicks();
	if (fireRateComp && fireRateComp->getNextFire() < currentTick)
	{
		fireRateComp->setLastFire(currentTick);
		glm::vec2 position = static_cast<TransformComponent*>(t_event.entity.getComponent(ComponentType::Transform))->getPos();
		static_cast<TransformComponent*>(m_playerBullets[m_nextPlayerBullet].entity.getComponent(ComponentType::Transform))->setPos(position);
		static_cast<ForceComponent*>(m_playerBullets[m_nextPlayerBullet].entity.getComponent(ComponentType::Force))->setForce(t_event.direction * t_event.forceScale);
		static_cast<HealthComponent*>(m_playerBullets[m_nextPlayerBullet].entity.getComponent(ComponentType::Health))->setHealth(1);
		static_cast<TimerComponent*>(m_playerBullets[m_nextPlayerBullet].entity.getComponent(ComponentType::Timer))->reset();
		m_playerBullets[m_nextPlayerBullet].type = t_event.type;

		m_nextPlayerBullet++;
		if (m_nextPlayerBullet >= BULLET_POOL_SIZE)
		{
			m_nextPlayerBullet = 0;
		}
	}

}

void ProjectileManager::createEnemyBullet(const createBulletEvent& t_event)
{
	glm::vec2 position = static_cast<TransformComponent*>(t_event.entity.getComponent(ComponentType::Transform))->getPos();
	static_cast<TransformComponent*>(m_enemyBullets[m_nextPlayerBullet].entity.getComponent(ComponentType::Transform))->setPos(position);
	static_cast<ForceComponent*>(m_enemyBullets[m_nextEnemyBullet].entity.getComponent(ComponentType::Force))->setForce(t_event.direction * t_event.forceScale);
	static_cast<HealthComponent*>(m_enemyBullets[m_nextEnemyBullet].entity.getComponent(ComponentType::Health))->setHealth(1);
	static_cast<TimerComponent*>(m_playerBullets[m_nextPlayerBullet].entity.getComponent(ComponentType::Timer))->reset();
	m_enemyBullets[m_nextEnemyBullet].type = t_event.type;

	m_nextEnemyBullet++;
	if (m_nextEnemyBullet >= BULLET_POOL_SIZE)
	{
		m_nextEnemyBullet = 0;
	}
}

void ProjectileManager::update(BaseSystem* t_system)
{
	for (auto& bullet : m_playerBullets)
	{
		if (static_cast<HealthComponent*>(bullet.entity.getComponent(ComponentType::Health))->getHealth() > 0)
		{
			t_system->update(bullet.entity);
		}
	}
	for (auto& bullet : m_enemyBullets)
	{
		if (static_cast<HealthComponent*>(bullet.entity.getComponent(ComponentType::Health))->getHealth() > 0)
		{
			t_system->update(bullet.entity);
		}
	}
}

void ProjectileManager::tick()
{
	for (auto& bullet : m_playerBullets)
	{
		if (!static_cast<TimerComponent*>(bullet.entity.getComponent(ComponentType::Timer))->tick(1))
		{
			static_cast<HealthComponent*>(bullet.entity.getComponent(ComponentType::Health))->setHealth(0);
		}
	}
	for (auto& bullet : m_enemyBullets)
	{
		if (!static_cast<TimerComponent*>(bullet.entity.getComponent(ComponentType::Timer))->tick(1))
		{
			static_cast<HealthComponent*>(bullet.entity.getComponent(ComponentType::Health))->setHealth(0);
		}
	}
}

void ProjectileManager::render(SDL_Renderer* t_renderer, RenderSystem* t_system)
{
	for (auto& bullet : m_playerBullets)
	{
		if (static_cast<HealthComponent*>(bullet.entity.getComponent(ComponentType::Health))->isAlive())
		{
			t_system->render(t_renderer, bullet.entity);
		}
	}
	for (auto& bullet : m_enemyBullets)
	{
		if (static_cast<HealthComponent*>(bullet.entity.getComponent(ComponentType::Health))->isAlive())
		{
			t_system->render(t_renderer, bullet.entity);
		}
	}
}
