#include "stdafx.h"
#include "PositionSystem.h"

void PositionSystem::addEntity(Entity* t_e)
{
	m_entities.push_back(t_e);

	std::vector<Component*> comps = t_e->getComponents();
	for (int i = 0; i < comps.size(); i++)
	{
		if (comps[i]->getType() == Component::ComponentType::Position)
		{
			m_posComps.push_back(static_cast<PositionComponent*>(comps[i]));
		}
		else if (comps[i]->getType() == Component::ComponentType::Input)
		{
			m_entitiesWithInput.push_back(m_entities.size() - 1);
			m_inputCompIndex.push_back(i);
		}
	}
	if (m_posComps.size() > m_entitiesWithInput.size())
	{
		m_entitiesWithInput.push_back(-1);
		m_inputCompIndex.push_back(-1);
	}
	comps.clear();
}

void PositionSystem::update()
{
	for (int i = 0; i < m_posComps.size(); i++)
	{
		if (m_posComps[i] != nullptr)
		{
			if (m_entitiesWithInput[i] > -1)
			{
				updateWithInput(i, dynamic_cast<InputComponent*>(m_entities[i]->getComponents()[m_inputCompIndex[i]]));
			}

			checkBorder(i);
		}
	}

#if (POS_SYS_DEBUG == 1 || POS_SYS_DEBUG == 2)
	//output every 1000ms
	if (SDL_TICKS_PASSED(SDL_GetTicks(), m_timer))
	{
		std::cout << "###############################################################################" << std::endl;
		for (int i = 0; i < m_entities.size(); i++)
		{
			std::cout << "Position of entity with id " << m_entities[i]->getId() << " [" << m_posComps[i]->getPos().x << ", " << m_posComps[i]->getPos().y << "]" << std::endl;
		}
		m_timer = SDL_GetTicks() + UPDATE_DELAY;
		std::cout << "###############################################################################" << std::endl;
	}
#endif

}

void PositionSystem::checkBorder(int t_index)
{
	if (m_posComps[t_index]->getPos().x > 825)
	{
		m_posComps[t_index]->setX(-25);
	}
	else if (m_posComps[t_index]->getPos().x < -25)
	{
		m_posComps[t_index]->setX(825);
	}
	if (m_posComps[t_index]->getPos().y > 625)
	{
		m_posComps[t_index]->setY(-25);
	}
	else if (m_posComps[t_index]->getPos().y < -25)
	{
		m_posComps[t_index]->setY(625);
	}
}

void PositionSystem::updateWithInput(int t_posIndex, InputComponent* t_input)
{
	if (t_input->m_keys.up)
	{
		m_posComps[t_posIndex]->moveUp();
	}
	if (t_input->m_keys.down)
	{
		m_posComps[t_posIndex]->moveDown();
	}
	if (t_input->m_keys.left)
	{
		m_posComps[t_posIndex]->moveLeft();
	}
	if (t_input->m_keys.right)
	{
		m_posComps[t_posIndex]->moveRight();
	}

#if (POS_SYS_DEBUG == 2)
	if (t_input->m_keys.up || t_input->m_keys.down || t_input->m_keys.left || t_input->m_keys.right)
	{
		std::cout << "=====================================================" << std::endl;
		std::cout << "Position of entity with id " << m_entities[t_posIndex]->getId() << " [" << m_posComps[t_posIndex]->getPos().x << ", " << m_posComps[t_posIndex]->getPos().y << "]" << std::endl;
		std::cout << "=====================================================" << std::endl;
	}
#endif
}
