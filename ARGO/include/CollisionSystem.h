#pragma once
#include "BaseSystem.h"
#include "ColliderAABBComponent.h"
#include "ColliderCircleComponent.h"
#include "TransformComponent.h"
#include "Utilities.h"
#include "QuadTree.h"

class CollisionSystem : public BaseSystem
{
public:
	CollisionSystem();
	void update(Entity& t_entity);
	void handleCollisions();

private:
	bool circleToCircleCollision(Entity* t_entityCircle1, Entity* t_entityCircle2);
	bool circleToAABBCollision(Entity* t_entityCircle, Entity* t_entityAABB);

	QuadTree m_quadTree;
	std::vector<Entity*> m_circleColliderBuffer;

	float m_seperationScaler;
};

