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

		switch (aiComp->getType()) {
		case eMelee:
			meleeAI(posComp,aiComp);
			break;
		case eRanged:
			rangedAI(posComp, aiComp);
		}
		
		//simpleMoveAi(posComp);
	}
}

//simple ai that moves around, remove it when we have proper ai
void AiSystem::simpleMoveAi(TransformComponent* t_posComp)
{
	if (t_posComp->m_movingRight)
	{
		t_posComp->moveRight();
	}
	else
	{
		t_posComp->moveLeft();
	}

	if (t_posComp->m_movingUp)
	{
		t_posComp->moveUp();
	}
	else
	{
		t_posComp->moveDown();
	}

	if (t_posComp->getPos().x >= 825)
	{
		t_posComp->m_movingRight = false;
	}
	else if (t_posComp->getPos().x <= -25)
	{
		t_posComp->m_movingRight = true;
	}
	if (t_posComp->getPos().y >= 625)
	{
		t_posComp->m_movingUp = true;
	}
	else if (t_posComp->getPos().y <= -25)
	{
		t_posComp->m_movingUp = false;
	}
}

void AiSystem::meleeAI(TransformComponent* t_posComp, AiComponent* t_aiComponent)
{
	//The Only Possible States Available to Melee Enemies. Use to limit Behaviours
	switch (t_aiComponent->getStates()) {
	case eSleeping:
		sleep(t_posComp, t_aiComponent);
		break;
	case eWander:
		wander(t_posComp,t_aiComponent);
	}
}

void AiSystem::rangedAI(TransformComponent* t_posComp, AiComponent* t_aiComponent)
{
	//The Only Possible States Available to Ranged Enemies. Use to limit Behaviours
	switch (t_aiComponent->getStates()) {
	case eSleeping:
		sleep(t_posComp, t_aiComponent);
		break;
	case eWander:
		wander(t_posComp, t_aiComponent);
	}
}

void AiSystem::wander(TransformComponent* t_posComp, AiComponent* t_aiComponent)
{
	float tempAdjuster = (((rand() % 20 + 1)));//Gives a number between 1 and 20
	tempAdjuster -= 10; //-10 to give a range of -9 and 10,
	tempAdjuster /= 10;  //divide by 10 to give a range of -0.9 and 1
	t_posComp->setRotation(t_posComp->getRotation() + (t_aiComponent->getMaxRotation() * tempAdjuster)); //Randomly changes current angle by the max which is scaled by the random scalar
	glm::vec2 tempvelocity = glm::vec2(glm::cos(t_posComp->getRotation() * M_PI / 180), sin(t_posComp->getRotation() * M_PI / 180));//Generates a unit vector in the given angle.
	tempvelocity *= glm::length(t_aiComponent->getMaxSpeed());//Scales it by the length of the max speed.
	auto tempPosition = t_posComp->getPos();
	tempPosition.x += tempvelocity.x;
	tempPosition.y += tempvelocity.y;
	t_posComp->setPos(tempPosition.x, tempPosition.y);//Updates Position
}

void AiSystem::sleep(TransformComponent* t_posComp, AiComponent* t_aiComponent)
{
	//Nothing will add code to awake unit if a target (i.e player) comes within range once discussions are had.
}
