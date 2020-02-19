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

void PhysicsSystem::update(Entity& t_entity/*float t_deltaTime*/) //deltaTime will be required here so it will need to be uncommented and used
{
	TransformComponent* posComp = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform));
	if (posComp)
	{
		ForceComponent* forceComp = static_cast<ForceComponent*>(t_entity.getComponent(ComponentType::Force));
		if (forceComp)
		{
			posComp->addPos(forceComp->getForce());
			if (forceComp->getHasFriction())
			{
				forceComp->setForce(forceComp->getForce() * FRICTION_SCALAR);
			}
		}

		checkBorder(posComp);
	}
}

void PhysicsSystem::checkBorder(TransformComponent* t_pos)
{
	if (t_pos->getPos().x > Utilities::LEVEL_TILE_WIDTH* Utilities::TILE_SIZE)
	{
		t_pos->setX(Utilities::LEVEL_TILE_WIDTH * Utilities::TILE_SIZE);
	}
	else if (t_pos->getPos().x < 0)
	{
		t_pos->setX(0);
	}
	if (t_pos->getPos().y > Utilities::LEVEL_TILE_HEIGHT* Utilities::TILE_SIZE)
	{
		t_pos->setY(Utilities::LEVEL_TILE_HEIGHT * Utilities::TILE_SIZE);
	}
	else if (t_pos->getPos().y < 0)
	{
		t_pos->setY(0);
	}
}

void PhysicsSystem::updateWithInput(const PhysicsMove& t_event)
{
	ForceComponent* forceComp = static_cast<ForceComponent*>(t_event.m_entity.getComponent(ComponentType::Force));
	if (forceComp)
	{
		forceComp->addForce(t_event.m_velocity);
	}
}
