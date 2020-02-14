#include "stdafx.h"
#include "Entity.h"
#include <iostream>

Entity::Entity()
{
	//m_components.reserve(S_MAX_COMPS);
	m_components.resize((int)ComponentType::Count);
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
	try
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
		case ComponentType::Force:
		{
			if (!m_components.at(COMPONENT_ID::FORCE_ID))
			{
				m_components.at(COMPONENT_ID::FORCE_ID) = t_c;
				return;
			}
			break;
		}
		case ComponentType::Text:
		{
			if (!m_components.at(COMPONENT_ID::TEXT_ID))
			{
				m_components.at(COMPONENT_ID::TEXT_ID) = t_c;
				return;
			}
			break;
		}
		case ComponentType::ColliderAABB:
		{
			if (!m_components.at(COMPONENT_ID::COLLIDER_AABB_ID))
			{
				m_components.at(COMPONENT_ID::COLLIDER_AABB_ID) = t_c;
				return;
			}
			break;
		}
		case ComponentType::ColliderCircle:
		{
			if (!m_components.at(COMPONENT_ID::COLLIDER_CIRCLE_ID))
			{
				m_components.at(COMPONENT_ID::COLLIDER_CIRCLE_ID) = t_c;
				return;
			}
			break;
		}
		case ComponentType::Timer:
		{
			if (!m_components.at(COMPONENT_ID::TIMER_ID))
			{
				m_components.at(COMPONENT_ID::TIMER_ID) = t_c;
				return;
			}
			break;
		}
		case ComponentType::Command:
		{
			if (!m_components.at(COMPONENT_ID::COMMAND_ID))
			{
				m_components.at(COMPONENT_ID::COMMAND_ID) = t_c;
				return;
			}
			break;
		}

		default:
			throw std::invalid_argument("Invalid component type!");
			break;
		}
		throw std::invalid_argument("already has this component!");
	}
	catch (const std::invalid_argument & e)
	{
		std::cerr << e.what() << std::endl;
		throw e;
	}
}

void Entity::removeCompType(ComponentType t_type)
{
	try
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
		case ComponentType::Force:
		{
			if (m_components.at(COMPONENT_ID::FORCE_ID))
			{
				delete m_components.at(COMPONENT_ID::FORCE_ID);
				m_components.at(COMPONENT_ID::FORCE_ID) = nullptr;
				return;
			}
			break;
		}
		case ComponentType::Text:
		{
			if (m_components.at(COMPONENT_ID::TEXT_ID))
			{
				delete m_components.at(COMPONENT_ID::TEXT_ID);
				m_components.at(COMPONENT_ID::TEXT_ID) = nullptr;
				return;
			}
			break;
		}
		case ComponentType::ColliderAABB:
		{
			if (!m_components.at(COMPONENT_ID::COLLIDER_AABB_ID))
			{
				delete m_components.at(COMPONENT_ID::COLLIDER_AABB_ID);

				m_components.at(COMPONENT_ID::COLLIDER_AABB_ID) = nullptr;

				return;
			}
			break;
		}
		case ComponentType::ColliderCircle:
		{
			if (!m_components.at(COMPONENT_ID::COLLIDER_CIRCLE_ID))
			{
				delete m_components.at(COMPONENT_ID::COLLIDER_CIRCLE_ID);

				m_components.at(COMPONENT_ID::COLLIDER_CIRCLE_ID) = nullptr;

				return;
			}
			break;
		}
		case ComponentType::Timer:
		{
			if (!m_components.at(COMPONENT_ID::TIMER_ID))
			{
				delete m_components.at(COMPONENT_ID::TIMER_ID);
				m_components.at(COMPONENT_ID::TIMER_ID) = nullptr;
				return;
			}
			break;
		}
		case ComponentType::Command:
		{
			if (!m_components.at(COMPONENT_ID::COMMAND_ID))
			{
				delete m_components.at(COMPONENT_ID::TIMER_ID);
				m_components.at(COMPONENT_ID::COMMAND_ID) = nullptr;
				return;
			}
			break;
		}
		default:
			throw std::invalid_argument("trying to delete an unknown component!");
			break;
		}
	}
	catch (const std::invalid_argument & e)
	{
		std::cerr << e.what() << std::endl;
		throw e;
	}
}

void Entity::removeAllComponents()
{
	for (int i = 0; i < m_components.size(); i++)
	{
		delete m_components.at(i);
		m_components.at(i) = nullptr;
	}
}

Component* Entity::getComponent(ComponentType t_type)
{
	try
	{
		switch (t_type)
		{
		case ComponentType::Health:
			return m_components.at(COMPONENT_ID::HEALTH_ID);
		case ComponentType::Transform:
			return m_components.at(COMPONENT_ID::TRANSFORM_ID);
		case ComponentType::Ai:
			return m_components.at(COMPONENT_ID::AI_ID);
		case ComponentType::Input:
			return m_components.at(COMPONENT_ID::INPUT_ID);
		case ComponentType::Visual:
			return m_components.at(COMPONENT_ID::VISUAL_ID);
		case ComponentType::Colour:
			return m_components.at(COMPONENT_ID::COLOUR_ID);
		case ComponentType::Force:
			return m_components.at(COMPONENT_ID::FORCE_ID);
		case ComponentType::Text:
			return m_components.at(COMPONENT_ID::TEXT_ID);
		case ComponentType::ColliderAABB:
			return m_components.at(COMPONENT_ID::COLLIDER_AABB_ID);
		case ComponentType::ColliderCircle:
			return m_components.at(COMPONENT_ID::COLLIDER_CIRCLE_ID);
		case ComponentType::Timer:
			return m_components.at(COMPONENT_ID::TIMER_ID);
		case ComponentType::Command:
			return m_components.at(COMPONENT_ID::COMMAND_ID);
		default:
			throw std::invalid_argument("trying to get an unknown component!");
			break;
		}
	}
	catch (const std::invalid_argument & e)
	{
		std::cerr << e.what() << std::endl;
		throw;
	}
}

std::vector<Component*>& Entity::getAllComps()
{
	return m_components;
}