#include "stdafx.h"
#include "Entity.h"
#include <iostream>

Entity::Entity()
{
	m_components.reserve(10);
}

Entity::~Entity()
{
	//std::cout << "Destroying entity" << std::endl;

	for (int i = m_components.size() - 1; i >= 0; i--)
	{
		delete m_components.at(i);
		m_components.at(i) = nullptr;
	}

	m_components.erase(std::remove(m_components.begin(), m_components.end(), nullptr), m_components.end());
}

void Entity::addComponent(Component* t_c)
{
	m_components.emplace_back(t_c);
}

void Entity::removeCompType(ComponentType t_type)
{
	for (std::vector<Component*>::iterator it = m_components.begin(); it != m_components.end(); )
	{
		if ((*it)->getType() == t_type)
		{
			delete* it;
			it = m_components.erase(it);
		}
		else
		{
			++it;
		}
	}
}

Component* Entity::getComponent(ComponentType t_type)
{
	for (std::vector<Component*>::iterator it = m_components.begin(); it != m_components.end(); )
	{
		if ((*it)->getType() == t_type)
		{
			return *it;
		}
		else
		{
			it++;
		}
	}

	return nullptr;
}

bool Entity::hasComponentType(ComponentType t_type) const
{
	for (auto& comp : m_components)
	{
		if (comp->getType() == t_type)
		{
			return true;
		}
	}

	return false;
}
