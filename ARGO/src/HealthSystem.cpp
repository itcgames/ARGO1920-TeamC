#include "stdafx.h"
#include "HealthSystem.h"

HealthSystem::~HealthSystem()
{
	BaseSystem::~BaseSystem();
}

void HealthSystem::update(Entity& t_entity)
{
	//if not nullptr
	if (t_entity.getAllComps().at(COMPONENT_ID::HEALTH_ID))
	{
		HealthComponent* hpComp = static_cast<HealthComponent*>(t_entity.getAllComps().at(COMPONENT_ID::HEALTH_ID));

		//if more than 0 hp
		if (hpComp->getHealth() > 0)
		{
			//reduce hp by 1
			hpComp->reduceHealth(1);
		}
		else //otherwise entity is dead
		{
			//mark entity as dead here?
#if (HP_SYS_DEBUG == 1)
			std::cout << "Entity is dead" << std::endl;
#endif // HP_SYS_DEBUG
		}
	}
}
