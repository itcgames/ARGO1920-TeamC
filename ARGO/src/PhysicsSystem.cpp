#include "stdafx.h"
#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem(EventManager& t_eventManager)
{
	t_eventManager.subscribe<PhysicsMove>(std::bind(&PhysicsSystem::updateWithInput, this, std::placeholders::_1));
}

PhysicsSystem::~PhysicsSystem()
{
	BaseSystem::~BaseSystem();
}

void PhysicsSystem::update(Entity& t_entity/*float t_deltaTime*/) //deltaTime will be required here so it will need to be uncommented and used
{
	if (t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))
	{
		TransformComponent* posComp = static_cast<TransformComponent*>(t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID));
		if (t_entity.getAllComps().at(COMPONENT_ID::FORCE_ID))
		{
			ForceComponent* forceComp = static_cast<ForceComponent*>(t_entity.getAllComps().at(COMPONENT_ID::FORCE_ID));

			posComp->addPos(forceComp->getForce());
			forceComp->setForce(forceComp->getForce() * FRICTION_SCALAR);
		}

		checkBorder(posComp);
	}
}

void PhysicsSystem::checkBorder(TransformComponent* t_pos)
{
	//magic numbers for window size will be replaced for world size
	//once we have a class that holds globals
	if (t_pos->getPos().x > 825)
	{
		t_pos->setX(-25);
	}
	else if (t_pos->getPos().x < -25)
	{
		t_pos->setX(825);
	}
	if (t_pos->getPos().y > 625)
	{
		t_pos->setY(-25);
	}
	else if (t_pos->getPos().y < -25)
	{
		t_pos->setY(625);
	}
}

void PhysicsSystem::updateWithInput(const PhysicsMove& t_event)
{
	if (t_event.m_entity.getAllComps().at(COMPONENT_ID::FORCE_ID))
	{
 		ForceComponent* forceComp = static_cast<ForceComponent*>(t_event.m_entity.getAllComps().at(COMPONENT_ID::FORCE_ID));
		/*if (t_event.m_velocity.y == -1)
		{
			forceComp->addForceY(-1.0f);

		}
		else if (t_event.m_velocity.y == 1)
		{
			forceComp->addForceY(1.0f);
		}
		else if (t_event.m_velocity.x == -1)
		{
			forceComp->addForceX(-1.0f);
		}
		else if (t_event.m_velocity.x == 1)
		{
			forceComp->addForceX(1.0f);
		}*/

		forceComp->addForce(t_event.m_velocity / 30000.0f);
	}  
}
