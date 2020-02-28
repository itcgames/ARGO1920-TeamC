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
		switch (weapon->getCurrent())
		{
		case Weapon::Shotgun:
			for (int pellet = 0; pellet < Utilities::NUMBER_OF_SHOTGUN_PELLETS; pellet++)
			{
				//This will output angle offsets of -30,-15,0,15,30 to get the correct spread.
				float angleOffset = -30.0f + (15.0f * pellet);
				m_projectileManager.createPlayerBullet(t_event, weapon->getCurrent(), angleOffset);
			}
			break;
		case Weapon::GrenadeLauncher:
		case Weapon::MachineGun:
		case Weapon::Pistol:
			m_projectileManager.createPlayerBullet(t_event, weapon->getCurrent());
			break;
		}

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
