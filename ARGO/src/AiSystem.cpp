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

		//switch statement to handle different ai types?
		//simpleMoveAi(posComp);
	}
}

//simple ai that moves around, remove it when we have proper ai
//void AiSystem::simpleMoveAi(TransformComponent* t_posComp)
//{
//	if (t_posComp->m_movingRight)
//	{
//		t_posComp->moveRight();
//	}
//	else
//	{
//		t_posComp->moveLeft();
//	}
//
//	if (t_posComp->m_movingUp)
//	{
//		t_posComp->moveUp();
//	}
//	else
//	{
//		t_posComp->moveDown();
//	}
//
//	if (t_posComp->getPos().x >= 825)
//	{
//		t_posComp->m_movingRight = false;
//	}
//	else if (t_posComp->getPos().x <= -25)
//	{
//		t_posComp->m_movingRight = true;
//	}
//	if (t_posComp->getPos().y >= 625)
//	{
//		t_posComp->m_movingUp = true;
//	}
//	else if (t_posComp->getPos().y <= -25)
//	{
//		t_posComp->m_movingUp = false;
//	}
//}
