#include "stdafx.h"
#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem(EventManager& t_eventManager)
{
	t_eventManager.subscribeToEvent<PhysicsMove>(std::bind(&PhysicsSystem::updateWithInput, this, std::placeholders::_1));
}

PhysicsSystem::~PhysicsSystem()
{
	BaseSystem::~BaseSystem();
}

void PhysicsSystem::update(Entity& t_entity, float t_dt)
{
	if (t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))
	{
		TransformComponent* posComp = static_cast<TransformComponent*>(t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID));
		if (t_entity.getAllComps().at(COMPONENT_ID::FORCE_ID))
		{
			ForceComponent* forceComp = static_cast<ForceComponent*>(t_entity.getAllComps().at(COMPONENT_ID::FORCE_ID));

			posComp->addPos(forceComp->getForce() * t_dt);
			if (forceComp->getHasFriction())
			{
				forceComp->setForce(forceComp->getForce() * FRICTION_SCALAR);
			}
		}

	}
}

void PhysicsSystem::update(Entity& t_entity)
{
}

void PhysicsSystem::updateWithInput(const PhysicsMove& t_event)
{
	if (t_event.m_entity.getAllComps().at(COMPONENT_ID::FORCE_ID))
	{
		ForceComponent* forceComp = static_cast<ForceComponent*>(t_event.m_entity.getAllComps().at(COMPONENT_ID::FORCE_ID));
		forceComp->addForce(t_event.m_velocity);
	}
}
