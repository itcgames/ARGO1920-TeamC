#include "stdafx.h"
#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem(EventManager& t_eventManager)
{
	t_eventManager.subscribeToEvent<PhysicsMove>(std::bind(&PhysicsSystem::updateWithInput, this, std::placeholders::_1));
	t_eventManager.subscribeToEvent<PhysicsRotate>(std::bind(&PhysicsSystem::updateRotation, this, std::placeholders::_1));
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
		// if -1 rotation is not set
		if (transformComp->getRightRotation() != -1)
		{
			transformComp->setRotation(transformComp->getRightRotation());
		}
		else if (transformComp->getLeftRotation() != -1)
		{
			transformComp->setRotation(transformComp->getLeftRotation());
		}
		// setting to -1 to check if they are not valid
		transformComp->setRightRotation(-1);
		transformComp->setLeftRotation(-1);
	}
}

void PhysicsSystem::update(Entity& t_entity)
{
}

void PhysicsSystem::updateWithInput(const PhysicsMove& t_event)
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
		float rotation = atan2f(t_event.velocity.y , t_event.velocity.x);
		rotation = glm::degrees(rotation);
		transformComp->setLeftRotation(rotation);
	}
}

void PhysicsSystem::updateRotation(const PhysicsRotate& t_event)
{ 
	TransformComponent* transformComp = static_cast<TransformComponent*>(t_event.entity.getComponent(ComponentType::Transform));
	if (transformComp)
	{
		float rotation = atan2f(t_event.rotation.y, t_event.rotation.x);
		rotation = glm::degrees(rotation);
		transformComp->setRightRotation(rotation);
	}
}
