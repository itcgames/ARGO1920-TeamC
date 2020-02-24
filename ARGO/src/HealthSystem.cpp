#include "stdafx.h"
#include "HealthSystem.h"

HealthSystem::~HealthSystem()
{
	BaseSystem::~BaseSystem();
}

void HealthSystem::update(Entity& t_entity)
{
	HealthComponent* hpComp = static_cast<HealthComponent*>(t_entity.getComponent(ComponentType::Health));
	//if not nullptr
	if (hpComp != nullptr)
	{
		//if more than 0 hp
		if (hpComp->getHealth() > 0)
		{
			//reduce hp by 1
			//hpComp->reduceHealth(1);
		}
		else //otherwise entity is dead
		{
			//mark entity as dead here?
#ifdef HP_SYS_DEBUG
			std::cout << "Entity is dead" << std::endl;
#endif // HP_SYS_DEBUG
		}
	}
}