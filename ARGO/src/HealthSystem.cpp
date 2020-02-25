#include "stdafx.h"
#include "HealthSystem.h"

HealthSystem::~HealthSystem()
{
	BaseSystem::~BaseSystem();
}

void HealthSystem::update(Entity& t_entity)
{
}

void HealthSystem::update(Entity& t_entity, float t_deltaTime)
{
	HealthComponent* hpComp = static_cast<HealthComponent*>(t_entity.getComponent(ComponentType::Health));
	//if not nullptr
	if (hpComp != nullptr)
	{
		//if more than 0 hp
		if (hpComp->getHealth() > 0)
		{
			hpComp->reduceInvincibilityCooldown(t_deltaTime);
		}
	}
}
