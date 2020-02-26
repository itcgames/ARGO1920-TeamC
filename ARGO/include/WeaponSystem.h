#pragma once
#include "BaseSystem.h"
#include "ProjectileManager.h"
#include "WeaponComponent.h"

class WeaponSystem :
	public BaseSystem
{
public:
	WeaponSystem(ProjectileManager& t_projectileManager, EventManager& t_eventManager);
	void update(Entity& t_entity);
	void update(Entity& t_entity, float t_deltaTime);
	void fireBullet(const CreateBulletEvent& t_event);
	void throwGlowStick(const CreateGlowStickEvent& t_event);
private:
	ProjectileManager& m_projectileManager;
};

