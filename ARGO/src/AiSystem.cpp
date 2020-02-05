#include "stdafx.h"
#include "AiSystem.h"

AiSystem::~AiSystem()
{
	BaseSystem::~BaseSystem();
}

void AiSystem::update(Entity& t_e)
{
	TransformComponent* posComp = dynamic_cast<TransformComponent*>(t_e.getComponent(ComponentType::Transform));
	//AiComponent* aiComp = dynamic_cast<AiComponent*>(t_e.getComponent(ComponentType::Ai));

	if (posComp)
	{
		if (posComp->m_movingRight)
		{
			posComp->moveRight();
		}
		else
		{
			posComp->moveLeft();
		}

		if (posComp->m_movingUp)
		{
			posComp->moveUp();
		}
		else
		{
			posComp->moveDown();
		}

		if (posComp->getPos().x >= 825)
		{
			posComp->m_movingRight = false;
		}
		else if (posComp->getPos().x <= -25)
		{
			posComp->m_movingRight = true;
		}
		if (posComp->getPos().y >= 625)
		{
			posComp->m_movingUp = true;
		}
		else if (posComp->getPos().y <= -25)
		{
			posComp->m_movingUp = false;
		}
	}
}