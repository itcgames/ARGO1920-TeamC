#include "AiSystem.h"

void AiSystem::addEntity(Entity* t_e)
{
	m_entities.push_back(t_e);

	//std::vector<Component*> comps = t_e->getComponents();
	//for (int i = 0; i < comps.size(); i++)
	//{
	//	if (comps[i]->getType() == Component::ComponentType::Position)
	//	{
	//		m_posComps.push_back(static_cast<PositionComponent*>(comps[i]));
	//	}
	//}
	//comps.clear();
}

void AiSystem::update()
{
	for (int i = 0; i < m_entities.size(); i++)
	{
		if (m_entities.at(i) != nullptr)
		{
			auto posComp = dynamic_cast<PositionComponent*>(m_entities[i]->getComponent(Component::ComponentType::Position));
			if (posComp->m_right)
			{
				posComp->moveRight();
			}
			else
			{
				posComp->moveLeft();
			}

			if (posComp->m_up)
			{
				posComp->moveUp();
			}
			else
			{
				posComp->moveDown();
			}

			if (posComp->getPos().x >= 825)
			{
				posComp->m_right = false;
			}
			else if (posComp->getPos().x <= -25)
			{
				posComp->m_right = true;
			}
			if (posComp->getPos().y >= 625)
			{
				posComp->m_up = true;
			}
			else if (posComp->getPos().y <= -25)
			{
				posComp->m_up = false;
			}
		}
	}
}