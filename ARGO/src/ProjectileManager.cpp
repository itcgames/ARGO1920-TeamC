#include "stdafx.h"
#include "ProjectileManager.h"

ProjectileManager::ProjectileManager(SDL_Renderer* t_renderer, EventManager& t_eventManager, glm::vec2& t_focusPoint, PhysicsSystem& t_physicsSystem, CollisionSystem& t_collisionSystem) :
	m_nextEnemyBullet(0),
	m_nextPlayerBullet(0),
	m_renderer(t_renderer),
	m_focusPoint(t_focusPoint),
	m_physicsSystem(t_physicsSystem),
	m_collisionSystem(t_collisionSystem)
{
	//t_eventManager.subscribeToEvent<CreateBulletEvent>(std::bind(&ProjectileManager::createPlayerBullet, this, std::placeholders::_1));
}

void ProjectileManager::init()
{
	m_audioMgr = AudioManager::Instance();

	for (auto& bullet : m_playerBullets)
	{
		bullet.entity.addComponent(new TransformComponent());
		bullet.entity.addComponent(new VisualComponent("bullet.png", m_renderer));
		bullet.entity.addComponent(new ForceComponent(glm::vec2(0, 0), false));
		bullet.entity.addComponent(new HealthComponent(1, 0));
		bullet.entity.addComponent(new ColliderCircleComponent(BULLET_RADIUS));
		bullet.entity.addComponent(new TimerComponent(BULLET_LIFETIME));
		bullet.entity.addComponent(new TagComponent(Tag::PlayerBullet));
	}
	for (auto& glowStick : m_glowsticks)
	{
		glowStick.addComponent(new TransformComponent());
		glowStick.addComponent(new VisualComponent("GlowStick.png", m_renderer));
		glowStick.addComponent(new ForceComponent(glm::vec2(0, 0)));
		glowStick.addComponent(new HealthComponent(1, 0));
		glowStick.addComponent(new ColliderCircleComponent(GLOWSTICK_RADIUS));
		glowStick.addComponent(new TimerComponent(BULLET_LIFETIME));
		glowStick.addComponent(new TagComponent(Tag::GlowStick));
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

void ProjectileManager::createPlayerBullet(const CreateBulletEvent& t_event, Weapon t_weapon)
{
	if (t_event.controller.getSDLController())
	{
		t_event.controller.activateRumble(RumbleStrength::Weak, RumbleLength::Short);
	}
	glm::vec2 position = static_cast<TransformComponent*>(t_event.entity.getComponent(ComponentType::Transform))->getPos();
	static_cast<TransformComponent*>(m_playerBullets[m_nextPlayerBullet].entity.getComponent(ComponentType::Transform))->setPos(position);
	static_cast<HealthComponent*>(m_playerBullets[m_nextPlayerBullet].entity.getComponent(ComponentType::Health))->setHealth(1);
	static_cast<TimerComponent*>(m_playerBullets[m_nextPlayerBullet].entity.getComponent(ComponentType::Timer))->reset();

	switch (t_weapon)
	{
	case Weapon::Pistol:
	case Weapon::MachineGun:
		m_playerBullets[m_nextPlayerBullet].entity.removeCompType(ComponentType::Visual);
		m_playerBullets[m_nextPlayerBullet].entity.addComponent(new VisualComponent("bullet.png", m_renderer));
		m_audioMgr->PlaySfx(Utilities::GUN_FIRE_PATH + "ak.wav");
		static_cast<ForceComponent*>(m_playerBullets[m_nextPlayerBullet].entity.getComponent(ComponentType::Force))->setForce(t_event.direction * PLAYER_BULLET_SPEED);
		break;
	case Weapon::GrenadeLauncher:
		m_playerBullets[m_nextPlayerBullet].entity.removeCompType(ComponentType::Visual);
		m_playerBullets[m_nextPlayerBullet].entity.addComponent(new VisualComponent("Grenade.png", m_renderer));
		m_audioMgr->PlaySfx(Utilities::GUN_FIRE_PATH + "launcher.wav");
		static_cast<ForceComponent*>(m_playerBullets[m_nextPlayerBullet].entity.getComponent(ComponentType::Force))->setForce(t_event.direction * PLAYER_GRENADE_SPEED);
		break;
	default:
		break;
	}

	m_playerBullets[m_nextPlayerBullet].type = t_event.type;

	m_nextPlayerBullet++;
	if (m_nextPlayerBullet >= BULLET_POOL_SIZE)
	{
		m_nextPlayerBullet = 0;
	}
}

void ProjectileManager::createGlowStick(const CreateGlowStickEvent& t_event)
{
	glm::vec2 position = static_cast<TransformComponent*>(t_event.entity.getComponent(ComponentType::Transform))->getPos();
	static_cast<TransformComponent*>(m_glowsticks[m_nextGlowStick].getComponent(ComponentType::Transform))->setPos(position);
	static_cast<HealthComponent*>(m_glowsticks[m_nextGlowStick].getComponent(ComponentType::Health))->setHealth(1);
	static_cast<TimerComponent*>(m_glowsticks[m_nextGlowStick].getComponent(ComponentType::Timer))->reset();
	static_cast<ForceComponent*>(m_glowsticks[m_nextGlowStick].getComponent(ComponentType::Force))->setForce(t_event.direction * GLOWSTICK_SPEED);

	m_nextGlowStick++;
	if (m_nextGlowStick >= Utilities::GLOWSTICK_POOL_SIZE)
	{
		m_nextGlowStick = 0;
	}
}

void ProjectileManager::createEnemyBullet(const CreateBulletEvent& t_event)
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

void ProjectileManager::update(float t_dt)
{
	tick(t_dt);
	for (auto& bullet : m_playerBullets)
	{
		updateBullet(bullet, t_dt);
	}
	for (auto& bullet : m_enemyBullets)
	{
		updateBullet(bullet, t_dt);
	}
	for (auto& glowStick : m_glowsticks)
	{
		updateGlowStick(glowStick, t_dt);
	}
}

void ProjectileManager::updateBullet(Bullet& t_bullet, float t_dt)
{
	HealthComponent* hpComp = static_cast<HealthComponent*>(t_bullet.entity.getComponent(ComponentType::Health));
	if (hpComp->isAlive())
	{
		m_physicsSystem.update(t_bullet.entity, t_dt);
		m_collisionSystem.update(t_bullet.entity);
	}
}

void ProjectileManager::updateGlowStick(Entity& t_glowStick, float t_dt)
{
	HealthComponent* hpComp = static_cast<HealthComponent*>(t_glowStick.getComponent(ComponentType::Health));
	if (hpComp->isAlive())
	{
		m_physicsSystem.update(t_glowStick, t_dt);
		m_collisionSystem.update(t_glowStick);
	}
}

void ProjectileManager::tick(float t_dt)
{
	try
	{
		for (auto& bullet : m_playerBullets)
		{
			if (!static_cast<TimerComponent*>(bullet.entity.getComponent(ComponentType::Timer))->tick(t_dt))
			{
				static_cast<HealthComponent*>(bullet.entity.getComponent(ComponentType::Health))->setHealth(0);
			}
		}
		for (auto& bullet : m_enemyBullets)
		{
			if (!static_cast<TimerComponent*>(bullet.entity.getComponent(ComponentType::Timer))->tick(t_dt))
			{
				static_cast<HealthComponent*>(bullet.entity.getComponent(ComponentType::Health))->setHealth(0);
			}
		}
		for (auto& glowStick : m_glowsticks)
		{
			if (!static_cast<TimerComponent*>(glowStick.getComponent(ComponentType::Timer))->tick(t_dt))
			{
				static_cast<HealthComponent*>(glowStick.getComponent(ComponentType::Health))->setHealth(0);
			}
		}
	}
	catch (const std::exception&)
	{
		throw std::invalid_argument("Timer / health component not found");
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
	for (auto& glowStick : m_glowsticks)
	{
		if (static_cast<HealthComponent*>(glowStick.getComponent(ComponentType::Health))->isAlive())
		{
			t_system->render(t_renderer, glowStick);
		}
	}
}

Entity(&ProjectileManager::getGlowsticks())[Utilities::GLOWSTICK_POOL_SIZE]
{
	return m_glowsticks;
}
