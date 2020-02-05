#include "Entity.h"
#include <iostream>

Entity::Entity(int t_entityIdNum) :
	m_id(t_entityIdNum)
{
	//m_components = new std::vector<Component*>;
	//m_components.reserve(5);
}

Entity::~Entity()
{
	std::cout << "Destroying entity" << std::endl;
	//for (int i = 0; i < m_components.size(); i++)
	//{
	//	delete m_components[i];
	//}
	//m_components.clear();
}

//void Entity::addComponent(Component* c)
//{
//	if (m_components.size() > 0)
//	{
//		for (int i = 0; i < m_components.size(); i++)
//		{
//			if (m_components[i]->getType() == c->getType())
//			{
//				break;
//			}
//			else if (i == m_components.size() - 1)
//			{
//				m_components.push_back(c);
//			}
//		}
//	}
//	else
//	{
//		m_components.push_back(c);
//	}
//}

void Entity::addSmartComponent(std::unique_ptr<Component> t_c)
{
	m_smartComps.emplace_back(std::move(t_c));
}

void Entity::removeComponent(Component* c)
{
	//for (int i = 0; i < m_components.size(); i++)
	//{
	//	if (m_components[i] != nullptr)
	//	{
	//		if (c == m_components[i])
	//		{
	//			delete m_components[i];
	//			m_components[i] = nullptr;
	//			break;
	//		}
	//	}
	//}
}

//std::vector<Component*> Entity::getComponents()
//{
//	return m_components;
//}

Component* Entity::getComponent(Component::ComponentType t_type)
{
	for (auto& comp : m_smartComps)
	{
		if (comp.get()->getType() == t_type)
		{
			return comp.get();
		}
	}
	return nullptr;
}
