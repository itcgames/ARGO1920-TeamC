#include "PositionSystem.h"

void PositionSystem::addEntity(Entity* t_e)
{
	m_entities.push_back(t_e);

	//std::vector<Component*> comps = t_e->getComponents();
	//for (int i = 0; i < comps.size(); i++)
	//{
	//	if (comps[i]->getType() == Component::ComponentType::Position)
	//	{
	//		m_posComps.push_back(static_cast<PositionComponent*>(comps[i]));
	//	}
	//	else if (comps[i]->getType() == Component::ComponentType::Input)
	//	{
	//		m_entitiesWithInput.push_back(m_entities.size() - 1);
	//		m_inputCompIndex.push_back(i);
	//	}
	//}
	//if (m_posComps.size() > m_entitiesWithInput.size())
	//{
	//	m_entitiesWithInput.push_back(-1);
	//	m_inputCompIndex.push_back(-1);
	//}
	//comps.clear();
}

void PositionSystem::update()
{
	for (int i = 0; i < m_entities.size(); i++)
	{
		if (m_entities[i] != nullptr)
		{
			auto comp = dynamic_cast<PositionComponent*>(m_entities[i]->getComponent(Component::ComponentType::Position));
			if (m_entities[i]->getComponent(Component::ComponentType::Input) != nullptr)
			{
				updateWithInput(comp, dynamic_cast<InputComponent*>(m_entities[i]->getComponent(Component::ComponentType::Input)));
			}
			checkBorder(comp);
		}
	}

//#if (POS_SYS_DEBUG == 1 || POS_SYS_DEBUG == 2)
//	//output every 1000ms
//	if (SDL_TICKS_PASSED(SDL_GetTicks(), m_timer))
//	{
//		std::cout << "###############################################################################" << std::endl;
//		for (int i = 0; i < m_entities.size(); i++)
//		{
//			std::cout << "Position of entity with id " << m_entities[i]->getId() << " [" << m_posComps[i]->getPos().x << ", " << m_posComps[i]->getPos().y << "]" << std::endl;
//		}
//		m_timer = SDL_GetTicks() + UPDATE_DELAY;
//		std::cout << "###############################################################################" << std::endl;
//	}
//#endif

}

void PositionSystem::checkBorder(PositionComponent* t_pos)
{

	if (t_pos->getPos().x > 825)
	{
		t_pos->setX(-25);
	}
	else if (t_pos->getPos().x < -25)
	{
		t_pos->setX(825);
	}
	if (t_pos->getPos().y > 625)
	{
		t_pos->setY(-25);
	}
	else if (t_pos->getPos().y < -25)
	{
		t_pos->setY(625);
	}
}

void PositionSystem::updateWithInput(PositionComponent* t_pos, InputComponent* t_input)
{
	if (t_input != nullptr)
	{
		if (t_input->m_keys.up)
		{
			t_pos->moveUp();
		}
		if (t_input->m_keys.down)
		{
			t_pos->moveDown();
		}
		if (t_input->m_keys.left)
		{
			t_pos->moveLeft();
		}
		if (t_input->m_keys.right)
		{
			t_pos->moveRight();
		}

//#if (POS_SYS_DEBUG == 2)
//		if (t_input->m_keys.up || t_input->m_keys.down || t_input->m_keys.left || t_input->m_keys.right)
//		{
//			std::cout << "=====================================================" << std::endl;
//			std::cout << "Position of entity with id " << m_entities[t_posIndex]->getId() << " [" << m_posComps[t_posIndex]->getPos().x << ", " << m_posComps[t_posIndex]->getPos().y << "]" << std::endl;
//			std::cout << "=====================================================" << std::endl;
//		}
//#endif
	}
}
