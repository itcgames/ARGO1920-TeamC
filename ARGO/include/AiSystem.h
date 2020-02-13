#pragma once
#include <iostream>
#include "SDL.h"
#include "Debug.h"
#include "BaseSystem.h"
#include "Entity.h"
#include "AiComponent.h"
#include "TransformComponent.h"
#include "AiStates.h"

class AiSystem : public BaseSystem
{
public:
	~AiSystem();
	void update(Entity& t_entity);
private:
	void simpleMoveAi(TransformComponent* t_posComp);
	void meleeAI(TransformComponent* t_posComp, AiComponent* t_aiComponent);
	void rangedAI(TransformComponent* t_posComp, AiComponent* t_aiComponent);
	void wander(TransformComponent* t_posComp, AiComponent* t_aiComponent);
	void sleep(TransformComponent* t_posComp, AiComponent* t_aiComponent);
};

