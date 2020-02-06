#include "stdafx.h"
#include "HealthSystem.h"

HealthSystem::~HealthSystem()
{
	BaseSystem::~BaseSystem();
}

void HealthSystem::update(Entity& t_e)
{
	auto hpComp = dynamic_cast<HealthComponent*>(t_e.getComponent(ComponentType::Health));

	//if not nullptr
	if (hpComp)
	{
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
			std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
			std::cout << "Entity with id " << m_entities[i]->getId() << " is dead. RIP [*]" << std::endl;
			std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
#endif // HP_SYS_DEBUG
		}
	}
}
