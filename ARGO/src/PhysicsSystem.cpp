#include "stdafx.h"
#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem(EventManager& t_eventManager)
{
	t_eventManager.subscribeToEvent<Events::PhysicsMove>(std::bind(&PhysicsSystem::updateWithInput, this, std::placeholders::_1));
	t_eventManager.subscribeToEvent<Events::PhysicsRotate>(std::bind(&PhysicsSystem::updateRotation, this, std::placeholders::_1));
}

PhysicsSystem::~PhysicsSystem()
{
	BaseSystem::~BaseSystem();
}

void PhysicsSystem::update(Entity& t_entity, float t_dt)
{
	TransformComponent* transformComp = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform));
	if (transformComp)
	{
		ForceComponent* forceComp = static_cast<ForceComponent*>(t_entity.getComponent(ComponentType::Force));
		if (forceComp)
		{
			transformComp->addPos(forceComp->getForce() * t_dt);
			if (forceComp->getHasFriction())
			{
				//this might have to be multiplied by t_dt
				forceComp->setForce(forceComp->getForce() * FRICTION_SCALAR);
			}
		}
		handleRotation(transformComp);
	}
}

void PhysicsSystem::update(Entity& t_entity)
{
}

void PhysicsSystem::updateWithInput(const Events::PhysicsMove& t_event)
{
	glm::vec2 normalisedVelocity = glm::normalize(t_event.velocity);
	ForceComponent* forceComp = static_cast<ForceComponent*>(t_event.entity.getComponent(ComponentType::Force));
	if (forceComp)
	{
		forceComp->addForce(normalisedVelocity);
	}
	TransformComponent* transformComp = static_cast<TransformComponent*>(t_event.entity.getComponent(ComponentType::Transform));
	if (transformComp)
	{
		transformComp->setLeftRotation(glm::degrees(atan2f(t_event.velocity.y, t_event.velocity.x)));
	}
}

void PhysicsSystem::updateRotation(const Events::PhysicsRotate& t_event)
{ 
	TransformComponent* transformComp = static_cast<TransformComponent*>(t_event.entity.getComponent(ComponentType::Transform));
	if (transformComp)
	{
 		transformComp->setRightRotation(glm::degrees(atan2f(t_event.rotation.y, t_event.rotation.x)));
	}
}

void PhysicsSystem::handleRotation(TransformComponent* t_transformComp)
{
	// if -1 rotation, it is not set
	if (t_transformComp->getRightRotation() != -1)
	{
		t_transformComp->setRotation(t_transformComp->getRightRotation());
	}
	else if (t_transformComp->getLeftRotation() != -1)
	{
		t_transformComp->setRotation(t_transformComp->getLeftRotation());
	}
	// setting to -1 to allow for check if they are not valid
	t_transformComp->setRightRotation(-1);
	t_transformComp->setLeftRotation(-1);
}
