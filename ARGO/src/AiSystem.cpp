#include "stdafx.h"
#include "AiSystem.h"

AiSystem::~AiSystem()
{
	BaseSystem::~BaseSystem();
}

void AiSystem::update(Entity& t_entity)
{
	TransformComponent* posComp = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform));
	AiComponent* aiComp = static_cast<AiComponent*>(t_entity.getComponent(ComponentType::Ai));

	//make sure that entity is not missing crucial components
	if (posComp && aiComp)
	{

		switch (aiComp->getType())
		{
		case AITypes::eMelee:
			meleeAI(posComp, aiComp);
			break;
		case AITypes::eRanged:
			rangedAI(posComp, aiComp);
			break;
		}
	}
}

void AiSystem::meleeAI(TransformComponent* t_posComp, AiComponent* t_aiComponent)
{
	//The Only Possible States Available to Melee Enemies. Use to limit Behaviours
	switch (t_aiComponent->getStates())
	{
	case AIStates::eSleeping:
		sleep(t_posComp, t_aiComponent);
		break;
	case AIStates::eWander:
		wander(t_posComp, t_aiComponent);
		break;
	}
}

void AiSystem::rangedAI(TransformComponent* t_posComp, AiComponent* t_aiComponent)
{
	//The Only Possible States Available to Ranged Enemies. Use to limit Behaviours
	switch (t_aiComponent->getStates())
	{
	case AIStates::eSleeping:
		sleep(t_posComp, t_aiComponent);
		break;
	case AIStates::eWander:
		wander(t_posComp, t_aiComponent);
		break;
	}
}

void AiSystem::wander(TransformComponent* t_posComp, AiComponent* t_aiComponent)
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
	t_posComp->setPos(t_posComp->getPos() + tempVelocity);
}

void AiSystem::sleep(TransformComponent* t_posComp, AiComponent* t_aiComponent)
{
	//Nothing will add code to awake unit if a target (i.e player) comes within range once discussions are had.
}