#include "stdafx.h"
#include "Entity.h"

Entity::Entity(int t_entityIdNum) :
	m_id(t_entityIdNum)
{
	//m_components = new std::vector<Component*>;
	m_components.reserve(5);
}

Entity::~Entity()
{
	for (int i = 0; i < m_components.size(); i++)
	{
		delete m_components[i];
	}
	m_components.clear();
}

void Entity::addComponent(Component* c)
{
	if (m_components.size() > 0)
	{
		for (int i = 0; i < m_components.size(); i++)
		{
			if (m_components[i]->getType() == c->getType())
			{
				break;
			}
			else if (i == m_components.size() - 1)
			{
				m_components.push_back(c);
			}
		}
	}
	else
	{
		m_components.push_back(c);
	}
}

void Entity::removeComponent(Component* c)
{
	for (int i = 0; i < m_components.size(); i++)
	{
		if (m_components[i] != nullptr)
		{
			if (c == m_components[i])
			{
				delete m_components[i];
				m_components[i] = nullptr;
				break;
			}
		}
	}
}

std::vector<Component*> Entity::getComponents()
{
	return m_components;
}
