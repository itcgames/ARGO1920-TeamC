#include "stdafx.h"
#include "Entity.h"
#include <iostream>

Entity::Entity()
{
}

//Entity::Entity(int t_entityIdNum) :
//	m_id(t_entityIdNum)
//{
//}

Entity::~Entity()
{
	//std::cout << "Destroying entity" << std::endl;

	m_components.clear();
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
