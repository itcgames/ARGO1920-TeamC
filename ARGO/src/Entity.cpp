#include "stdafx.h"
#include "Entity.h"
#include <iostream>

Entity::Entity()
{
	//m_components.reserve(S_MAX_COMPS);
	m_components.resize(S_MAX_COMPS);
}

Entity::~Entity()
{
	for (int i = m_components.size() - 1; i >= 0; i--)
	{
		delete m_components.at(i);
		m_components.at(i) = nullptr;
	}

	m_components.erase(std::remove(m_components.begin(), m_components.end(), nullptr), m_components.end());
}

void Entity::addComponent(Component* t_c)
{
	switch (t_c->getType())
	{
	case ComponentType::Health:
	{
		if (!m_components.at(COMPONENT_ID::HEALTH_ID))
		{
			m_components.at(COMPONENT_ID::HEALTH_ID) = t_c;
			return;
		}
		break;
	}
	case ComponentType::Transform:
	{
		if (!m_components.at(COMPONENT_ID::TRANSFORM_ID))
		{
			m_components.at(COMPONENT_ID::TRANSFORM_ID) = t_c;
			return;
		}
		break;
	}
	case ComponentType::Ai:
	{
		if (!m_components.at(COMPONENT_ID::AI_ID))
		{
			m_components.at(COMPONENT_ID::AI_ID) = t_c;
			return;
		}
		break;
	}
	case ComponentType::Input:
	{
		if (!m_components.at(COMPONENT_ID::INPUT_ID))
		{
			m_components.at(COMPONENT_ID::INPUT_ID) = t_c;
			return;
		}
		break;
	}
	case ComponentType::Visual:
	{
		if (!m_components.at(COMPONENT_ID::VISUAL_ID))
		{
			m_components.at(COMPONENT_ID::VISUAL_ID) = t_c;
			return;
		}
		break;
	}
	case ComponentType::Colour:
	{
		if (!m_components.at(COMPONENT_ID::COLOUR_ID))
		{
			m_components.at(COMPONENT_ID::COLOUR_ID) = t_c;
			return;
		}
		break;
	}
	default:

		break;
	}

	throw std::invalid_argument("already has this component!");
}

void Entity::removeCompType(ComponentType t_type)
{
	switch (t_type)
	{
	case ComponentType::Health:
	{
		if (m_components.at(COMPONENT_ID::HEALTH_ID))
		{
			delete m_components.at(COMPONENT_ID::HEALTH_ID);
			m_components.at(COMPONENT_ID::HEALTH_ID) = nullptr;
			return;
		}
		break;
	}
	case ComponentType::Transform:
	{
		if (m_components.at(COMPONENT_ID::TRANSFORM_ID))
		{
			delete m_components.at(COMPONENT_ID::TRANSFORM_ID);
			m_components.at(COMPONENT_ID::TRANSFORM_ID) = nullptr;
			return;
		}
		break;
	}
	case ComponentType::Ai:
	{
		if (m_components.at(COMPONENT_ID::AI_ID))
		{
			delete m_components.at(COMPONENT_ID::AI_ID);
			m_components.at(COMPONENT_ID::AI_ID) = nullptr;
			return;
		}
		break;
	}
	case ComponentType::Input:
	{
		if (m_components.at(COMPONENT_ID::INPUT_ID))
		{
			delete m_components.at(COMPONENT_ID::INPUT_ID);
			m_components.at(COMPONENT_ID::INPUT_ID) = nullptr;
			return;
		}
		break;
	}
	case ComponentType::Visual:
	{
		if (m_components.at(COMPONENT_ID::VISUAL_ID))
		{
			delete m_components.at(COMPONENT_ID::VISUAL_ID);
			m_components.at(COMPONENT_ID::VISUAL_ID) = nullptr;
			return;
		}
		break;
	}
	case ComponentType::Colour:
	{
		if (m_components.at(COMPONENT_ID::COLOUR_ID))
		{
			delete m_components.at(COMPONENT_ID::COLOUR_ID);
			m_components.at(COMPONENT_ID::COLOUR_ID) = nullptr;
			return;
		}
		break;
	}
	default:
		throw std::invalid_argument("trying to delete an unknown component!");
		break;
	}	
}

Component* Entity::getComponent(ComponentType t_type)
{
	for (std::vector<Component*>::iterator it = m_components.begin(); it != m_components.end(); )
	{
		if ((*it) && (*it)->getType() == t_type)
		{
			return *it;
		}
		else
		{
			++it;
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

std::vector<Component*>& Entity::getAllComps()
{
	return m_components;
}
