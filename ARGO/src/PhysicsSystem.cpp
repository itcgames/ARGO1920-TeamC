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
	TransformComponent* posComp = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform));
	if (posComp)
	{
		ForceComponent* forceComp = static_cast<ForceComponent*>(t_entity.getComponent(ComponentType::Force));
		if (forceComp)
		{
			posComp->addPos(forceComp->getForce() * t_dt);
			if (forceComp->getHasFriction())
			{
				//this might have to be multiplied by t_dt
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
	ForceComponent* forceComp = static_cast<ForceComponent*>(t_event.m_entity.getComponent(ComponentType::Force));
	if (forceComp)
	{
		forceComp->addForce(t_event.m_velocity);
	}
}
