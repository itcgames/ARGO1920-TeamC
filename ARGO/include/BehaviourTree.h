#pragma once
#include <Entity.h>
#include "LevelManager.h"


////----------------------------Helper functions.----------------------------
static void move(ForceComponent* t_forceComp, TransformComponent* t_transformComp, glm::vec2 t_direction)
{
	t_forceComp->addForce(t_direction);
	t_transformComp->setRotation(glm::degrees(atan2(t_direction.y, t_direction.x)));
}
static void path(ForceComponent* t_forceComp, TransformComponent* t_transformComp, glm::vec2 t_destination, LevelManager& t_levelManager)
{
	std::vector<glm::vec2> path = t_levelManager.createPath(t_transformComp->getPos(), t_destination);

	if (!path.empty())
	{
		glm::vec2 direction = glm::normalize(path.back() - t_transformComp->getPos());
		t_forceComp->addForce(direction);
		t_transformComp->setRotation(glm::degrees(atan2(direction.y, direction.x)));
	}
}

////----------------------------This is the data we will need for our behaviour to function.----------------------------
const int CAN_SEE_ENEMY_DISTANCE = Utilities::TILE_SIZE * 6;
const int DISTANCE_TO_CARE_ABOUT_GOAL_SCALER = Utilities::TILE_SIZE * 2;
const int DEFAULT_MOVE_TOWARD_GOAL_WEIGHT = 5;
const float RETREAT_WEIGHT_SCALER = 10.0f;
const float COLLECT_PICKUP_SCALER = 8.0f;

struct EnemyData
{
	Entity* enemy;
	float distance;
	int nearbyEnemies;
};
struct ClosestLeaderData
{
	Entity* entity;
	float distance;
};
struct ClosestPickupData
{
	Entity* entity;
	float distance;
	PickupType type;
};
struct ClosestHealthData
{
	Entity* entity;
	float distance;
};
struct GoalData
{
	glm::vec2 destination;
};

//----------------------------These are the basic classes needed for the behaviour tree to function----------------------------
class WeightedNode 
{
public:
	virtual bool run(Entity& t_entity) = 0;
	virtual float getWeight(Entity& t_entity) = 0;
};

class WeightedCompositeNode : public WeightedNode 
{
private:
	std::vector<WeightedNode*> children;
public:
	const std::vector<WeightedNode*>& getChildren() const { return children; }
	void addChild(WeightedNode* child) { children.emplace_back(child); }
};

class WeightedSelector : public WeightedCompositeNode 
{
public:
	virtual bool run(Entity& t_entity) override
	{
		WeightedNode* bestNode = nullptr;
		float bestWeight = -1.0f;
		for (WeightedNode* child : getChildren()) 
		{
			float weight = child->getWeight(t_entity);
			if (weight > bestWeight)
			{
				bestNode = child;
				bestWeight = weight;
			}
		}
		if (bestNode)
		{
			bestNode->run(t_entity);
			return true;
		}
		return false;
	}
	virtual float getWeight(Entity& t_entity) override
	{
		return 0.0f;
	}
};

//----------------------------These are the the nodes that represent the different behaviour the tree is capable of----------------------------
class RetreatBehaviour : public WeightedNode {
private:
	EnemyData* m_data;
	LevelManager& m_levelManager;
public:
	RetreatBehaviour(EnemyData* t_data, LevelManager& t_levelManager) : m_data(t_data), m_levelManager(t_levelManager) {}
	virtual bool run(Entity& t_entity) override
	{
		TransformComponent* transformComp = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform));
		ForceComponent* forceComp = static_cast<ForceComponent*>(t_entity.getComponent(ComponentType::Force));
		glm::vec2 enemyPos = static_cast<TransformComponent*>(m_data->enemy->getComponent(ComponentType::Transform))->getPos();

		if (forceComp)
		{
			move(forceComp, transformComp, glm::normalize(transformComp->getPos() - enemyPos));
			return true;
		}
		return false;
	}
	virtual float getWeight(Entity& t_entity) override
	{
		//inverting the scaler by taking it away from 1.
		return (1 - m_data->distance / (CAN_SEE_ENEMY_DISTANCE * CAN_SEE_ENEMY_DISTANCE)) * RETREAT_WEIGHT_SCALER;
	}
};

class HoldBehaviour : public WeightedNode {
private:
	EnemyData* m_data;
	LevelManager& m_levelManager;
public:
	HoldBehaviour(EnemyData* t_data, LevelManager& t_levelManager) : m_data(t_data), m_levelManager(t_levelManager) {}
	virtual bool run(Entity& t_entity) override
	{
		return true;
	}
	virtual float getWeight(Entity& t_entity) override
	{
		return m_data->nearbyEnemies + 1.0f;
	}
};

class GetAmmoBehaviour : public WeightedNode {
private:
	ClosestPickupData* m_data;
	LevelManager& m_levelManager;
public:
	GetAmmoBehaviour(ClosestPickupData* t_data, LevelManager& t_levelManager) : m_data(t_data), m_levelManager(t_levelManager) {}
	virtual bool run(Entity& t_entity) override
	{
		TransformComponent* transformComp = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform));
		ForceComponent* forceComp = static_cast<ForceComponent*>(t_entity.getComponent(ComponentType::Force));
		TransformComponent* transComp = static_cast<TransformComponent*>(m_data->entity->getComponent(ComponentType::Transform));
		if (forceComp && transComp->getPos() != transformComp->getPos())
		{
			path(forceComp, transformComp, transComp->getPos(), m_levelManager);
			return true;
		}
		return false;
	}
	virtual float getWeight(Entity& t_entity) override
	{
		WeaponComponent* weapon = static_cast<WeaponComponent*>(t_entity.getComponent(ComponentType::Weapon));
		float ammoScaler = 1;
		if (weapon->getMaxAmmo() != 0)
		{
			//inverting the scaler by taking it away from 1.
			ammoScaler = 1 - (weapon->getAmmo() / weapon->getMaxAmmo());
		}
		//inverting the scaler by taking it away from 1.
		return (1 - m_data->distance / (CAN_SEE_ENEMY_DISTANCE * CAN_SEE_ENEMY_DISTANCE)) * ammoScaler * COLLECT_PICKUP_SCALER;
	}
};

class GetHealthBehaviour : public WeightedNode {
private:
	ClosestHealthData* m_data;
	LevelManager& m_levelManager;
public:
	GetHealthBehaviour(ClosestHealthData* t_data, LevelManager& t_levelManager) : m_data(t_data), m_levelManager(t_levelManager) {}
	virtual bool run(Entity& t_entity) override
	{
		TransformComponent* transformComp = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform));
		ForceComponent* forceComp = static_cast<ForceComponent*>(t_entity.getComponent(ComponentType::Force));
		TransformComponent* transComp = static_cast<TransformComponent*>(m_data->entity->getComponent(ComponentType::Transform));
		if (forceComp && transComp->getPos() != transformComp->getPos())
		{
			path(forceComp, transformComp, transComp->getPos(), m_levelManager);
			return true;
		}
		return false;
	}
	virtual float getWeight(Entity& t_entity) override
	{
		HealthComponent* health = static_cast<HealthComponent*>(t_entity.getComponent(ComponentType::Health));
		float healthScaler = 1;
		if (health->getHealth() != 0)
		{
			//inverting the scaler by taking it away from 1.
			healthScaler = 1 - (health->getHealth() / health->getMaxHealth());
		}
		//inverting the scaler by taking it away from 1.
		return (1 - m_data->distance / (CAN_SEE_ENEMY_DISTANCE * CAN_SEE_ENEMY_DISTANCE)) * healthScaler * COLLECT_PICKUP_SCALER;
	}
};

class MoveToGoalBehaviour : public WeightedNode {
private:
	GoalData* m_data;
	LevelManager& m_levelManager;
public:
	MoveToGoalBehaviour(GoalData* t_data, LevelManager& t_levelManager) : m_data(t_data), m_levelManager(t_levelManager) {}
	virtual bool run(Entity& t_entity) override
	{
		TransformComponent* transformComp = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform));
		ForceComponent* forceComp = static_cast<ForceComponent*>(t_entity.getComponent(ComponentType::Force));
		if (forceComp && m_data->destination != transformComp->getPos())
		{
			path(forceComp, transformComp, m_data->destination, m_levelManager);
			return true;
		}
		return false;
	}
	virtual float getWeight(Entity& t_entity) override
	{
		AiComponent* aiComp = static_cast<AiComponent*>(t_entity.getComponent(ComponentType::Ai));
	
		if (aiComp->getIsleader())
		{
			return 5.0f;
		}
	
		return 0.0f;
	}
};

class MoveToLeaderBehaviour : public WeightedNode {
private:
	ClosestLeaderData* m_data;
	LevelManager& m_levelManager;
public:
	MoveToLeaderBehaviour(ClosestLeaderData* t_data, LevelManager& t_levelManager) : m_data(t_data), m_levelManager(t_levelManager) {}
	virtual bool run(Entity& t_entity) override
	{
		TransformComponent* transformComp = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform));
		ForceComponent* forceComp = static_cast<ForceComponent*>(t_entity.getComponent(ComponentType::Force));
		TransformComponent* goalTransComp = static_cast<TransformComponent*>(m_data->entity->getComponent(ComponentType::Transform));
		if (forceComp && goalTransComp->getPos() != transformComp->getPos())
		{
			path(forceComp, transformComp, goalTransComp->getPos(), m_levelManager);
			return true;
		}
		return false;
	}
	virtual float getWeight(Entity& t_entity) override
	{
		return m_data->distance == 0 ? 0.0f : m_data->distance / (DISTANCE_TO_CARE_ABOUT_GOAL_SCALER * DISTANCE_TO_CARE_ABOUT_GOAL_SCALER);
	}
};
