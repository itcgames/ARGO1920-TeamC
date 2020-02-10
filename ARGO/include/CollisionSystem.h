#pragma once
#include "BaseSystem.h"
#include "ColliderAABB.h"
#include "ColliderCircle.h"
#include "TransformComponent.h"
#include "Utilities.h"

class CollisionSystem :	public BaseSystem
{
public:
	CollisionSystem();
	void update(Entity& t_entity);
	void handleCollisions();

private:
	bool circleToCircleCollision(Entity* t_entity1, Entity* t_entity2);
	bool circleToAABBCollision(Entity* t_entityCircle, Entity* t_entityAABB);

	std::vector<Entity*> m_circleColliderBuffer;
	std::vector<Entity*> m_aabbColliderBuffer;
};

