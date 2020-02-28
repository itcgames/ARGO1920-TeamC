#pragma once
#include <iostream>
#include "SDL.h"
#include "Debug.h"
#include "BaseSystem.h"
#include "Entity.h"
#include "AiComponent.h"
#include "TransformComponent.h"
#include "ForceComponent.h"
#include "HealthComponent.h"
#include "FlowFieldComponent.h"
#include "LevelManager.h"
#include "AiStates.h"
#include "Utilities.h"
#include "BehaviourTree.h"
#include <queue>

struct BotWaypoints
{
	glm::vec2 destination;
	bool searched;
};

class AiSystem : public BaseSystem
{
public:
	AiSystem(Entity(&t_players)[Utilities::S_MAX_PLAYERS], Entity(&t_enemies)[Utilities::ENEMY_POOL_SIZE], Entity(&t_pickups)[Utilities::PICKUP_POOL_SIZE], Entity& t_goal, EventManager& t_eventManager, LevelManager& t_levelManager);
	~AiSystem();
	void update(Entity& t_entity);
private:
	void simpleMoveAi(TransformComponent* t_posComp);
	void meleeAI(TransformComponent* t_posComp, AiComponent* t_aiComponent, ForceComponent* t_forceComponent);
	void rangedAI(TransformComponent* t_posComp, AiComponent* t_aiComponent, ForceComponent* t_forceComponent);
	void seek(TransformComponent* t_posComp, AiComponent* t_aiComponent, ForceComponent* t_forceComponent, glm::vec2 t_destination);
	void wander(TransformComponent* t_posComp, AiComponent* t_aiComponent, ForceComponent* t_forceComponent);
	void sleep(TransformComponent* t_posComp, AiComponent* t_aiComponent, ForceComponent* t_forceComponent);

	void playerAI(Entity& t_entity);
	void playerMovementDecision(Entity& t_entity);
	void playerShootingDecision(Entity& t_entity);

	void setEnemyData(glm::vec2 t_botPosition);
	void setClosestLeaderData(glm::vec2 t_botPosition);
	void setClosestPickupData(glm::vec2 t_botPosition);
	void setGoalData(glm::vec2 t_botPosition);
	void setLeader();
	void setCurrentWaypoint();



	Entity(&m_players)[Utilities::S_MAX_PLAYERS];
	Entity(&m_enemies)[Utilities::ENEMY_POOL_SIZE];
	Entity(&m_pickups)[Utilities::PICKUP_POOL_SIZE];
	Entity& m_goal;
	const static int TOTAL_WAYPOINTS = 5;
	BotWaypoints m_waypoints[TOTAL_WAYPOINTS];
	int m_currentWaypoint;

	EventManager& m_eventManager;
	LevelManager& m_levelmanager;

	WeightedSelector m_behaviourTree;

	EnemyData m_botEnemyData;
	ClosestLeaderData m_botLeaderData;
	ClosestPickupData m_botPickupData;
	ClosestHealthData m_botHealthPickupData;
	GoalData m_botGoalData;
};

