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
#include "BehaviourTree.h"

class AiSystem : public BaseSystem
{
public:
	AiSystem(Entity(&t_players)[Utilities::S_MAX_PLAYERS], std::vector<Entity>& t_enemies);
	~AiSystem();
	void update(Entity& t_entity);
private:
	void simpleMoveAi(TransformComponent* t_posComp);
	void meleeAI(TransformComponent* t_posComp, AiComponent* t_aiComponent, ForceComponent* t_forceComponent);
	void rangedAI(TransformComponent* t_posComp, AiComponent* t_aiComponent, ForceComponent* t_forceComponent);
	void wander(TransformComponent* t_posComp, AiComponent* t_aiComponent, ForceComponent* t_forceComponent);
	void sleep(TransformComponent* t_posComp, AiComponent* t_aiComponent, ForceComponent* t_forceComponent);

	void playerAI(Entity& t_entity);
	glm::vec2& playerMovementDecision(Entity& t_entity);
	void playerShootingDecision(Entity& t_entity, glm::vec2& t_closestEnemyPosition);

	void setEnemyData(glm::vec2 t_botPosition);
	void setClosestLeaderData(glm::vec2 t_botPosition);
	void setClosestPickupData(glm::vec2 t_botPosition);
	void setGoalData(glm::vec2 t_botPosition);

	Entity(&m_players)[Utilities::S_MAX_PLAYERS];
	std::vector<Entity>& m_enemies;

	WeightedSelector m_behaviourTree;

	EnemyData m_botEnemyData;
	ClosestLeaderData m_botLeaderData;
	ClosestPickupData m_botPickupData;
	GoalData m_botGoalData;
};

