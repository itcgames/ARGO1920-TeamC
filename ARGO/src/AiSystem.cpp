#include "stdafx.h"
#include "AiSystem.h"

void AiSystem::addEntity(Entity* t_e)
{
	m_entities.push_back(t_e);

	std::vector<Component*> comps = t_e->getComponents();
	for (int i = 0; i < comps.size(); i++)
	{
		if (comps[i]->getType() == Component::ComponentType::Position)
		{
			m_posComps.push_back(static_cast<PositionComponent*>(comps[i]));
		}
	}
	comps.clear();
}

void AiSystem::update()
{
	for (int i = 0; i < m_posComps.size(); i++)
	{
		if (m_posComps[i]->m_right)
		{
			m_posComps[i]->moveRight();
		}
		else
		{
			m_posComps[i]->moveLeft();
		}

		if (m_posComps[i]->m_up)
		{
			m_posComps[i]->moveUp();
		}
		else
		{
			m_posComps[i]->moveDown();
		}

		if (m_posComps[i]->getPos().x >= 825)
		{
			m_posComps[i]->m_right = false;
		}
		else if (m_posComps[i]->getPos().x <= -25)
		{
			m_posComps[i]->m_right = true;
		}
		if (m_posComps[i]->getPos().y >= 625)
		{
			m_posComps[i]->m_up = true;
		}
		else if (m_posComps[i]->getPos().y <= -25)
		{
			m_posComps[i]->m_up = false;
		}

	}
}