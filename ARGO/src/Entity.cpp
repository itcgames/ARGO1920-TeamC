#include "stdafx.h"
#include "Entity.h"
#include <iostream>

Entity::Entity()
{
	m_components.resize(static_cast<int>(ComponentType::Count));
}

Entity::~Entity()
{
	for (int i = m_components.size() - 1; i >= 0; i--)
	{
		if (m_components.at(i) == NULL)
		{
			continue;
		}
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

		case ComponentType::Primitive:
		{
			if (!m_components.at(COMPONENT_ID::PRIMITIVE_ID))
			{
				m_components.at(COMPONENT_ID::PRIMITIVE_ID) = t_c;
				return;
			}
			break;
		}

		case ComponentType::ParticleEmitter:
		{
			if (!m_components.at(COMPONENT_ID::PARTICLE_ID))
			{
				m_components.at(COMPONENT_ID::PARTICLE_ID) = t_c;
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
		case ComponentType::Tag:
		{
			if (!m_components.at(COMPONENT_ID::TAG_ID))
			{
				m_components.at(COMPONENT_ID::TAG_ID) = t_c;
				return;
			}
			break;
		}
		case ComponentType::PickUp:
		{
			if (!m_components.at(COMPONENT_ID::PICK_UP_ID))
			{
				m_components.at(COMPONENT_ID::PICK_UP_ID) = t_c;
				return;
			}
			break;
		}
		case ComponentType::Tile:
		{
			if (!m_components.at(COMPONENT_ID::TILE_ID))
			{
				m_components.at(COMPONENT_ID::TILE_ID) = t_c;
				return;
			}
			break;
		}
		case ComponentType::HUD:
		{
			if (!m_components.at(COMPONENT_ID::HUD_ID))
			{
				m_components.at(COMPONENT_ID::HUD_ID) = t_c;
				return;
			}
			break;
		}
		case ComponentType::FireRate:
		{
			if (!m_components.at(COMPONENT_ID::FIRE_RATE_ID))
			{
				m_components.at(COMPONENT_ID::FIRE_RATE_ID) = t_c;
				return;
			}
			break;
		}
		case ComponentType::FlowField:
		{
			if (!m_components.at(COMPONENT_ID::FLOW_FIELD_ID))
			{
				m_components.at(COMPONENT_ID::FLOW_FIELD_ID) = t_c;
				return;
			}
			break;
		}
		case ComponentType::LightField:
		{
			if (!m_components.at(COMPONENT_ID::LIGHT_FIELD_ID))
			{
				m_components.at(COMPONENT_ID::LIGHT_FIELD_ID) = t_c;
				return;
			}
			break;
		}
		case ComponentType::Weapon:
		{
			if (!m_components.at(COMPONENT_ID::WEAPON_ID))
			{
				m_components.at(COMPONENT_ID::WEAPON_ID) = t_c;
				return;
			}
			break;
		}
		case ComponentType::Pathing:
		{
			if (!m_components.at(COMPONENT_ID::PATHING_ID))
			{
				m_components.at(COMPONENT_ID::PATHING_ID) = t_c;
				return;
			}
			break;
		}
		case ComponentType::FSM:
		{
			if (!m_components.at(COMPONENT_ID::FSM_ID))
			{
				m_components.at(COMPONENT_ID::FSM_ID) = t_c;
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
	catch (const std::invalid_argument& e)
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
		case ComponentType::Primitive:
		{
			if (m_components.at(COMPONENT_ID::PRIMITIVE_ID))
			{
				delete m_components.at(COMPONENT_ID::PRIMITIVE_ID);
				m_components.at(COMPONENT_ID::PRIMITIVE_ID) = nullptr;
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
		case ComponentType::ParticleEmitter:
		{
			if (m_components.at(COMPONENT_ID::PARTICLE_ID))
			{
				delete m_components.at(COMPONENT_ID::PARTICLE_ID);
				m_components.at(COMPONENT_ID::PARTICLE_ID) = nullptr;
				return;
			}
			break;
		}
		case ComponentType::PickUp:
		{
			if (m_components.at(COMPONENT_ID::PICK_UP_ID))
			{
				delete m_components.at(COMPONENT_ID::PICK_UP_ID);
				m_components.at(COMPONENT_ID::PICK_UP_ID) = nullptr;
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
			if (m_components.at(COMPONENT_ID::COLLIDER_AABB_ID))
			{
				delete m_components.at(COMPONENT_ID::COLLIDER_AABB_ID);

				m_components.at(COMPONENT_ID::COLLIDER_AABB_ID) = nullptr;

				return;
			}
			break;
		}
		case ComponentType::ColliderCircle:
		{
			if (m_components.at(COMPONENT_ID::COLLIDER_CIRCLE_ID))
			{
				delete m_components.at(COMPONENT_ID::COLLIDER_CIRCLE_ID);

				m_components.at(COMPONENT_ID::COLLIDER_CIRCLE_ID) = nullptr;

				return;
			}
			break;
		}
		case ComponentType::Timer:
		{
			if (m_components.at(COMPONENT_ID::TIMER_ID))
			{
				delete m_components.at(COMPONENT_ID::TIMER_ID);
				m_components.at(COMPONENT_ID::TIMER_ID) = nullptr;
				return;
			}
			break;
		}
		case ComponentType::Command:
		{
			if (m_components.at(COMPONENT_ID::COMMAND_ID))
			{
				delete m_components.at(COMPONENT_ID::TIMER_ID);
				m_components.at(COMPONENT_ID::COMMAND_ID) = nullptr;
				return;
			}
			break;
		}
		case ComponentType::HUD:
		{
			if (m_components.at(COMPONENT_ID::HUD_ID))
			{
				delete m_components.at(COMPONENT_ID::HUD_ID);
				m_components.at(COMPONENT_ID::HUD_ID) = nullptr;
				return;
			}
			break;
		}
		case ComponentType::Tag:
		{
			if (m_components.at(COMPONENT_ID::TAG_ID))
			{
				delete m_components.at(COMPONENT_ID::TAG_ID);
				m_components.at(COMPONENT_ID::TAG_ID) = nullptr;
				return;
			}
			break;
		}
		case ComponentType::Tile:
		{
			if (m_components.at(COMPONENT_ID::TILE_ID))
			{
				delete m_components.at(COMPONENT_ID::TILE_ID);
				m_components.at(COMPONENT_ID::TILE_ID) = nullptr;
				return;
			}
			break;
		}
		case ComponentType::FireRate:
		{
			if (m_components.at(COMPONENT_ID::FIRE_RATE_ID))
			{
				delete m_components.at(COMPONENT_ID::FIRE_RATE_ID);
				m_components.at(COMPONENT_ID::FIRE_RATE_ID) = nullptr;
				return;
			}
			break;
		}
		case ComponentType::FlowField:
		{
			if (m_components.at(COMPONENT_ID::FLOW_FIELD_ID))
			{
				delete m_components.at(COMPONENT_ID::FLOW_FIELD_ID);
				m_components.at(COMPONENT_ID::FLOW_FIELD_ID) = nullptr;
				return;
			}
			break;
		}
		case ComponentType::LightField:
		{
			if (m_components.at(COMPONENT_ID::LIGHT_FIELD_ID))
			{
				delete m_components.at(COMPONENT_ID::LIGHT_FIELD_ID);
				m_components.at(COMPONENT_ID::LIGHT_FIELD_ID) = nullptr;
				return;
			}
			break;
		}
		case ComponentType::Weapon:
		{
			if (m_components.at(COMPONENT_ID::WEAPON_ID))
			{
				delete m_components.at(COMPONENT_ID::WEAPON_ID);
				m_components.at(COMPONENT_ID::WEAPON_ID) = nullptr;
				return;
			}
			break;
		}
		case ComponentType::Pathing:
		{
			if (m_components.at(COMPONENT_ID::PATHING_ID))
			{
				delete m_components.at(COMPONENT_ID::PATHING_ID);
				m_components.at(COMPONENT_ID::PATHING_ID) = nullptr;
				return;
			}
			break;
		}
		case ComponentType::FSM:
		{
			if (m_components.at(COMPONENT_ID::FSM_ID))
			{
				delete m_components.at(COMPONENT_ID::FSM_ID);
				m_components.at(COMPONENT_ID::FSM_ID) = nullptr;
				return;
			}
			break;
		}
		default:
			throw std::invalid_argument("trying to delete an unknown component!");
			break;
		}
	}
	catch (const std::invalid_argument& e)
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

void Entity::nullAllComponents()
{
	for (int i = 0; i < m_components.size(); i++)
	{
		m_components.at(i) = nullptr;
	}
}

Component* Entity::getComponent(ComponentType t_type) const
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
		case ComponentType::Tag:
			return m_components.at(COMPONENT_ID::TAG_ID);
		case ComponentType::Tile:
			return m_components.at(COMPONENT_ID::TILE_ID);
		case ComponentType::Primitive:
			return m_components.at(COMPONENT_ID::PRIMITIVE_ID);
		case ComponentType::ParticleEmitter:
			return m_components.at(COMPONENT_ID::PARTICLE_ID);
		case ComponentType::PickUp:
			return m_components.at(COMPONENT_ID::PICK_UP_ID);
		case ComponentType::FireRate:
			return m_components.at(COMPONENT_ID::FIRE_RATE_ID);
		case ComponentType::HUD:
			return m_components.at(COMPONENT_ID::HUD_ID);
		case ComponentType::FlowField:
			return m_components.at(COMPONENT_ID::FLOW_FIELD_ID);
		case ComponentType::LightField:
			return m_components.at(COMPONENT_ID::LIGHT_FIELD_ID);
		case ComponentType::Weapon:
			return m_components.at(COMPONENT_ID::WEAPON_ID);
		case ComponentType::Pathing:
			return m_components.at(COMPONENT_ID::PATHING_ID);
		case ComponentType::FSM:
			return m_components.at(COMPONENT_ID::FSM_ID);
		default:
			throw std::invalid_argument("trying to get an unknown component!");
			break;
		}
	}
	catch (const std::invalid_argument& e)
	{
		std::cerr << e.what() << std::endl;
		throw;
	}
}