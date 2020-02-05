#include "InputSystem.h"

void InputSystem::addEntity(Entity* t_e)
{
	m_entities.push_back(t_e);

	//std::vector<Component*> comps = t_e->getComponents();
	//for (int i = 0; i < comps.size(); i++)
	//{
	//	if (comps[i]->getType() == Component::ComponentType::Input)
	//	{
	//		m_inputComps.push_back(dynamic_cast<InputComponent*>(comps[i]));
	//	}
	//}
	//comps.clear();
}

void InputSystem::update()
{
	for (int i = 0; i < m_entities.size(); i++)
	{
		if (m_entities.at(i) != nullptr)
		{
#if (INPUT_SYS_DEBUG == 1)
			std::cout << "Updating input for entity with id: " << m_entities[i]->getId() << std::endl;
#endif
			dynamic_cast<InputComponent*>(m_entities.at(i)->getComponent(Component::ComponentType::Input))->update();
		}
	}
}
