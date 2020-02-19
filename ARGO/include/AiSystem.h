#pragma once
#include <iostream>
#include "SDL.h"
#include "Debug.h"
#include "BaseSystem.h"
#include "Entity.h"
#include "AiComponent.h"
#include "TransformComponent.h"
#include "ForceComponent.h"
#include "AiStates.h"
#include "Utilities.h"

class AiSystem : public BaseSystem
{
public:
	~AiSystem();
	void update(Entity& t_entity);
private:
	void simpleMoveAi(TransformComponent* t_posComp);
	void meleeAI(TransformComponent* t_posComp, AiComponent* t_aiComponent, ForceComponent* t_forceComponent);
	void rangedAI(TransformComponent* t_posComp, AiComponent* t_aiComponent, ForceComponent* t_forceComponent);
	void wander(TransformComponent* t_posComp, AiComponent* t_aiComponent, ForceComponent* t_forceComponent);
	void sleep(TransformComponent* t_posComp, AiComponent* t_aiComponent, ForceComponent* t_forceComponent);

	void playerAI(Entity& t_entity);
	void playerMovementDecision(Entity& t_entity);
	void playerShootingDecision(Entity& t_entity);

	//Entity (&m_players)[Utilities::]
};

