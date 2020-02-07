#include "stdafx.h"
#include "PhysicsSystem.h"

PhysicsSystem::~PhysicsSystem()
{
	BaseSystem::~BaseSystem();
}

void PhysicsSystem::update(Entity& t_entity/*float t_deltaTime*/) //deltaTime will be required here so it will need to be uncommented and used
{
	if (t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))
	{
		TransformComponent* posComp = static_cast<TransformComponent*>(t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID));
		//check if entity has an input comp
		if (t_entity.getAllComps().at(COMPONENT_ID::INPUT_ID) && t_entity.getAllComps().at(COMPONENT_ID::FORCE_ID))
		{
			InputComponent* inputComp = static_cast<InputComponent*>(t_entity.getAllComps().at(COMPONENT_ID::INPUT_ID));
			ForceComponent* forceComp = static_cast<ForceComponent*>(t_entity.getAllComps().at(COMPONENT_ID::FORCE_ID));
			updateWithInput(forceComp, inputComp);
		}

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

void PhysicsSystem::updateWithInput(ForceComponent* t_force, InputComponent* t_input)
{
	if (t_input->m_keys.up)
	{
		t_force->addForceY(-1.0f);
	}
	if (t_input->m_keys.down)
	{
		t_force->addForceY(1.0f);
	}
	if (t_input->m_keys.left)
	{
		t_force->addForceX(-1.0f);
	}
	if (t_input->m_keys.right)
	{
		t_force->addForceX(1.0f);
	}
}
