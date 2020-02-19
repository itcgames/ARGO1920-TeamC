#include "stdafx.h"
#include "AiSystem.h"

AiSystem::~AiSystem()
{
	BaseSystem::~BaseSystem();
}

void AiSystem::update(Entity& t_entity)
{

	//make sure that entity is not missing crucial components
	if (t_entity.getAllComps().at(COMPONENT_ID::AI_ID) && t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))
	{
		TransformComponent* posComp = static_cast<TransformComponent*>(t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID));
		AiComponent* aiComp = static_cast<AiComponent*>(t_entity.getAllComps().at(COMPONENT_ID::AI_ID));
		ForceComponent* forceComp = static_cast<ForceComponent*>(t_entity.getComponent(ComponentType::Force));

		switch (aiComp->getType())
		{
		case AITypes::eMelee:
			meleeAI(posComp, aiComp, forceComp);
			break;
		case AITypes::eRanged:
			rangedAI(posComp, aiComp, forceComp);
			break;
		case AITypes::ePlayerBot:
			rangedAI(posComp, aiComp, forceComp);
			break;
		}
	}
}

void AiSystem::meleeAI(TransformComponent* t_posComp, AiComponent* t_aiComponent, ForceComponent* t_forceComponent)
{
	//The Only Possible States Available to Melee Enemies. Use to limit Behaviours
	switch (t_aiComponent->getStates())
	{
	case AIStates::eSleeping:
		sleep(t_posComp, t_aiComponent, t_forceComponent);
		break;
	case AIStates::eWander:
		wander(t_posComp, t_aiComponent, t_forceComponent);
		break;
	}
}

void AiSystem::rangedAI(TransformComponent* t_posComp, AiComponent* t_aiComponent, ForceComponent* t_forceComponent)
{
	//The Only Possible States Available to Ranged Enemies. Use to limit Behaviours
	switch (t_aiComponent->getStates())
	{
	case AIStates::eSleeping:
		sleep(t_posComp, t_aiComponent, t_forceComponent);
		break;
	case AIStates::eWander:
		wander(t_posComp, t_aiComponent, t_forceComponent);
		break;
	}
}

void AiSystem::playerAI(Entity& t_entity)
{
	//get a decision on movement

	//get a decision on shooting
}

void AiSystem::playerMovementDecision(Entity& t_entity)
{
	//check for closest humie player. (Need to hold reference to all non ai players) otherwise the first ai in the list of ai players is set as the leader.
	//set goal to leader/closest player
	
	//get ammo weight is dependent on the amount of ammo the ai has and how close it is to an ammo pack
	//hold position weight is dependent on the amount of enemies close to the player.
	//retreat wieght is based on the distance to the closest enemy
	//if not leader:
	//	move toward goal weight is dependant in  on how far away the goal is
	//if leader: move toward goal is a fixed value 

	//create a path towards the the largest weight.
}

void AiSystem::playerShootingDecision(Entity& t_entity)
{
	//if enemy in range (pew pew)
}

void AiSystem::wander(TransformComponent* t_posComp, AiComponent* t_aiComponent, ForceComponent* t_forceComponent)
{
	//Gives a number between 1 and 20, -10 change the range to -9 and 10 then divides it by 10 to give a range of -.9 and 1.
	float tempAdjuster = (((rand() % 20 + 1)));
	tempAdjuster = (tempAdjuster - 10) / 10;

	//Randomly changes current angle by the max which is scaled by the random scalar
	t_posComp->setRotation(t_posComp->getRotation() + (t_aiComponent->getMaxRotation() * tempAdjuster));
	//Generates a unit vector in the given angle.
	glm::vec2 tempVelocity = glm::vec2(glm::cos(t_posComp->getRotation() * M_PI / 180), glm::sin(t_posComp->getRotation() * M_PI / 180));
	//Scales the Unit Vector by the length of the max speed.
	tempVelocity *= glm::length(t_aiComponent->getMaxSpeed());
	//Updates Position
	t_forceComponent->addForce(tempVelocity);
	//t_posComp->setPos(t_posComp->getPos() + tempVelocity);
}

void AiSystem::sleep(TransformComponent* t_posComp, AiComponent* t_aiComponent, ForceComponent* t_forceComponent)
{
	//Nothing will add code to awake unit if a target (i.e player) comes within range once discussions are had.
}