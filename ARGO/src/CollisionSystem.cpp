#include "stdafx.h"
#include "CollisionSystem.h"

CollisionSystem::CollisionSystem()
{
	m_aabbColliderBuffer.reserve(100);
	m_circleColliderBuffer.reserve(100);
}

void CollisionSystem::update(Entity& t_entity)
{
	//if not nullptr
	if (t_entity.getAllComps().at(COMPONENT_ID::COLLIDER_AABB_ID) &&
		t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))
	{
		m_aabbColliderBuffer.push_back(&t_entity);
	}
	else if (t_entity.getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID) &&
		t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))
	{
		m_circleColliderBuffer.push_back(&t_entity);
	}
}

void CollisionSystem::handleCollisions()
{
	for (int i = 0; i < m_circleColliderBuffer.size(); i++)
	{
		if (i + 1 < m_circleColliderBuffer.size())
		{
			for (int j = i + 1; j < m_circleColliderBuffer.size(); j++)
			{
				if (circleToCircleCollision(m_circleColliderBuffer[i], m_circleColliderBuffer[j]))
				{
					//add collision functionality here ie. move them away from each other
				}
			}
		}

		for (int j = 0; j < m_aabbColliderBuffer.size(); j++)
		{
			if (circleToAABBCollision(m_circleColliderBuffer[i], m_aabbColliderBuffer[j]))
			{
				//add collision functionality here ie. move them away from each other
			}
		}
	}
	m_circleColliderBuffer.clear();
	m_aabbColliderBuffer.clear();
}

bool CollisionSystem::circleToCircleCollision(Entity* t_entity1, Entity* t_entity2)
{
	glm::vec2 position1 = static_cast<TransformComponent*>(t_entity1->getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos();
	int radius1 = static_cast<ColliderCircle*>(t_entity1->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();
	glm::vec2 position2 = static_cast<TransformComponent*>(t_entity2->getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos();
	int radius2 = static_cast<ColliderCircle*>(t_entity2->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();

	glm::vec2 between = position1 - position2;
	return sqrt(between.x * between.x + between.y * between.y) < radius1 + radius2;
}

bool CollisionSystem::circleToAABBCollision(Entity* t_entityCircle, Entity* t_entityAABB)
{
	glm::vec2 circlePosition = static_cast<TransformComponent*>(t_entityCircle->getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos();
	int radius = static_cast<ColliderCircle*>(t_entityCircle->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();
	glm::vec2 rectPosition = static_cast<TransformComponent*>(t_entityAABB->getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos();
	glm::vec2 bounds = static_cast<ColliderAABB*>(t_entityAABB->getAllComps().at(COMPONENT_ID::COLLIDER_AABB_ID))->getBounds();

	glm::vec2 between = circlePosition - rectPosition;
	if (sqrt(between.x * between.x + between.y * between.y) < radius + Utilities::TILE_SIZE * sqrt(2) / 2)
	{
		int DeltaX = circlePosition.x - std::max(rectPosition.x, std::min(circlePosition.x, rectPosition.x + bounds.x));
		int DeltaY = circlePosition.y - std::max(rectPosition.y, std::min(circlePosition.y, rectPosition.y + bounds.y));
		return (DeltaX * DeltaX + DeltaY * DeltaY) < (radius * radius);
	}
	return false;
}
