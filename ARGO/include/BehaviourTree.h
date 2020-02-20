#pragma once
#include <Entity.h>

////----------------------------This is the data we will need for our behaviour to function.----------------------------
const int BOT_CAN_SEE_ENEMY_DISTANCE = Utilities::TILE_SIZE * 8;
const int BOT_DISTANCE_TO_CARE_ABOUT_GOAL_SCALER = Utilities::TILE_SIZE * 2;
const int BOT_DEFAUL_MOVE_TOWARD_GOAL_WEIGHT = 5;

struct EnemyData
{
	Entity* entity;
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
};
struct GoalData
{
	Entity* entity;
	float distance;
};

//----------------------------These are the basic classes needed for the behaviour tree to function----------------------------
class WeightedNode 
{
public:
	virtual bool run(Entity& t_entity) = 0;
	virtual float getWeight() = 0;
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
			float weight = child->getWeight();
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
	virtual float getWeight() override 
	{
		return 0.0f;
	}
};

//----------------------------These are the the nodes that represent the different behaviour the tree is capable of----------------------------
class RetreatBehaviour : public WeightedNode {
private:
	EnemyData* m_data;
public:
	RetreatBehaviour(EnemyData* t_data) : m_data(t_data) {}
	virtual bool run(Entity& t_entity) override
	{
		glm::vec2 pos = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform))->getPos();
		ForceComponent* forceComp = static_cast<ForceComponent*>(t_entity.getComponent(ComponentType::Force));
		glm::vec2 enemyPos = static_cast<TransformComponent*>(m_data->entity->getComponent(ComponentType::Transform))->getPos();

		if (forceComp)
		{
			glm::vec2 betweenGoalAndEnemy = enemyPos - pos;
			forceComp->addForce(glm::normalize(pos - betweenGoalAndEnemy));
			return true;
		}
		return false;
	}
	virtual float getWeight() override
	{
		return (1 - m_data->distance / (BOT_CAN_SEE_ENEMY_DISTANCE * BOT_CAN_SEE_ENEMY_DISTANCE)) * 10;
	}
};

class HoldBehaviour : public WeightedNode {
private:
	EnemyData* m_data;
public:
	HoldBehaviour(EnemyData* t_data) : m_data(t_data) {}
	virtual bool run(Entity& t_entity) override
	{
		return true;
	}
	virtual float getWeight() override
	{
		return m_data->nearbyEnemies + 1.0f;
	}
};

class GetAmmoBehaviour : public WeightedNode { //TODO
private:
	ClosestPickupData* m_data;
public:
	GetAmmoBehaviour(ClosestPickupData* t_data) : m_data(t_data) {}
	virtual bool run(Entity& t_entity) override
	{
		return false;
	}
	virtual float getWeight() override
	{
		return 0.0f;
	}
};

class MoveToGoalBehaviour : public WeightedNode { //TODO
private:
	GoalData* m_data;
public:
	MoveToGoalBehaviour(GoalData* t_data) : m_data(t_data) {}
	virtual bool run(Entity& t_entity) override
	{
		return false;
	}
	virtual float getWeight() override
	{
		return 0.0f;
	}
};

class MoveToLeaderBehaviour : public WeightedNode {
private:
	ClosestLeaderData* m_data;
public:
	MoveToLeaderBehaviour(ClosestLeaderData* t_data) : m_data(t_data) {}
	virtual bool run(Entity& t_entity) override
	{
		glm::vec2 pos = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform))->getPos();
		ForceComponent* forceComp = static_cast<ForceComponent*>(t_entity.getComponent(ComponentType::Force));
		TransformComponent* goalTransComp = static_cast<TransformComponent*>(m_data->entity->getComponent(ComponentType::Transform));
		if (forceComp)
		{
			forceComp->addForce(glm::normalize(goalTransComp->getPos() - pos));
			return true;
		}
		return false;
	}
	virtual float getWeight() override
	{
		return m_data->distance / (BOT_DISTANCE_TO_CARE_ABOUT_GOAL_SCALER * BOT_DISTANCE_TO_CARE_ABOUT_GOAL_SCALER);
	}
};