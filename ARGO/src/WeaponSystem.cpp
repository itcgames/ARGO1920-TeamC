#include "stdafx.h"
#include "WeaponSystem.h"

WeaponSystem::WeaponSystem(ProjectileManager& t_projectileManager, EventManager& t_eventManager) :
	m_projectileManager(t_projectileManager)
{
	t_eventManager.subscribeToEvent<CreateBulletEvent>(std::bind(&WeaponSystem::fireBullet, this, std::placeholders::_1));
	t_eventManager.subscribeToEvent<CreateGlowStickEvent>(std::bind(&WeaponSystem::throwGlowStick, this, std::placeholders::_1));
}

void WeaponSystem::update(Entity& t_entity)
{
}

void WeaponSystem::update(Entity& t_entity, float t_deltaTime)
{
	static_cast<WeaponComponent*>(t_entity.getComponent(ComponentType::Weapon))->reduceCooldowns(t_deltaTime);
}

void WeaponSystem::fireBullet(const CreateBulletEvent& t_event)
{
	WeaponComponent* weapon = static_cast<WeaponComponent*>(t_event.entity.getComponent(ComponentType::Weapon));
	if (weapon && weapon->fireGun())
	{
		m_projectileManager.createPlayerBullet(t_event, weapon->getCurrent());
	}
}

void WeaponSystem::throwGlowStick(const CreateGlowStickEvent& t_event)
{
	WeaponComponent* weapon = static_cast<WeaponComponent*>(t_event.entity.getComponent(ComponentType::Weapon));
	if (weapon && weapon->throwGlowStick())
	{
		m_projectileManager.createGlowStick(t_event);
	}
}
