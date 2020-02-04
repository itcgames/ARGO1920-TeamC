#include "stdafx.h"
#include "HealthSystem.h"

void HealthSystem::addEntity(Entity* t_e)
{
	m_entities.push_back(t_e);

	m_timer = SDL_GetTicks() + UPDATE_DELAY;

	std::vector<Component*> comps = t_e->getComponents();
	for (int i = 0; i < comps.size(); i++)
	{
		if (comps[i]->getType() == Component::ComponentType::Health)
		{
			m_hpComps.push_back(static_cast<HealthComponent*>(comps[i]));
		}
	}
		comps.clear();
}

void HealthSystem::update()
{
	//update every 500ms
	if (SDL_TICKS_PASSED(SDL_GetTicks(), m_timer))
	{
		//loop through every component
		for (int i = 0; i < m_hpComps.size(); i++)
		{
			if (m_hpComps[i])
			{
				//if more than 0 hp
				if (m_hpComps[i]->getHealth() > 0)
				{
					//reduce hp by 1
					m_hpComps[i]->reduceHealth(1);
				}
				else //otherwise entity is dead
				{
#if (HP_SYS_DEBUG == 1)
					std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
					std::cout << "Entity with id " << m_entities[i]->getId() << " is dead. RIP [*]" << std::endl;
					std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
#endif // HP_SYS_DEBUG
					m_entities[i]->removeComponent(m_hpComps[i]);
					//delete m_hpComps[i]; //dont try to delete what the entity has deleted already
					m_hpComps[i] = nullptr;
				}
			}
		}

//output health values when debugging
#if (HP_SYS_DEBUG == 1)
		if (m_hpComps.size() > 0)
		{
			bool printTopBorder = true;
			bool printBotBorder = false;

			for (int i = 0; i < m_entities.size(); i++)
			{
				if (m_hpComps[i])
				{
					if (printTopBorder)
					{
						std::cout << "********************************" << std::endl;
						printTopBorder = false;
						printBotBorder = true;
					}
					std::cout << "Entity with id " << m_entities[i]->getId() << " has " << m_hpComps[i]->getHealth() << " health" << std::endl;
				}

			}
			if (printBotBorder)
			{
				std::cout << "********************************" << std::endl;
			}
		}
#endif
		//reset the timer
		m_timer = SDL_GetTicks() + UPDATE_DELAY;
	}
}
