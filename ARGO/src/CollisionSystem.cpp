#include "stdafx.h"
#include "CollisionSystem.h"

CollisionSystem::CollisionSystem() :
	m_quadTree(0, glm::vec2(0, 0), glm::vec2(Utilities::LEVEL_TILE_WIDTH * Utilities::TILE_SIZE, Utilities::LEVEL_TILE_HEIGHT * Utilities::TILE_SIZE)),
	m_seperationScaler(30)
{
	m_circleColliderBuffer.reserve(100);
}

void CollisionSystem::update(Entity& t_entity)
{
	//if not nullptr
	if (t_entity.getAllComps().at(COMPONENT_ID::COLLIDER_AABB_ID) &&
		t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))
	{
		glm::vec2 rectPosition = static_cast<TransformComponent*>(t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos();
		glm::vec2 bounds = static_cast<ColliderAABBComponent*>(t_entity.getAllComps().at(COMPONENT_ID::COLLIDER_AABB_ID))->getBounds();
		Quad quad{ &t_entity, rectPosition, bounds };
		m_quadTree.insert(quad);
	}
	else if (t_entity.getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID) &&
		t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))
	{
		m_circleColliderBuffer.push_back(&t_entity);
		glm::vec2 rectPosition = static_cast<TransformComponent*>(t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos();
		int radius = static_cast<ColliderCircleComponent*>(t_entity.getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();
		glm::vec2 bounds{ radius, radius };
		Quad quad{ &t_entity, rectPosition - bounds, bounds * 2.0f };
		m_quadTree.insert(quad);
	}
}

void CollisionSystem::handleCollisions()
{
	for (int i = 0; i < m_circleColliderBuffer.size(); i++)
	{
		TransformComponent* rectPosition = static_cast<TransformComponent*>(m_circleColliderBuffer[i]->getAllComps().at(COMPONENT_ID::TRANSFORM_ID));
		int radius = static_cast<ColliderCircleComponent*>(m_circleColliderBuffer[i]->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();
		glm::vec2 bounds{ radius, radius };
		Quad quad{ NULL, rectPosition->getPos() - bounds, bounds * 2.0f };

		std::vector<Entity*> entities;
		m_quadTree.retrieve(&entities, quad);

		for (auto& entity : entities)
		{
			TransformComponent* otherPosition = static_cast<TransformComponent*>(entity->getAllComps().at(COMPONENT_ID::TRANSFORM_ID));
			if (entity != m_circleColliderBuffer[i])
			{
				if (entity->getAllComps().at(COMPONENT_ID::COLLIDER_AABB_ID) && circleToAABBCollision(m_circleColliderBuffer.at(i), entity))
				{
					//handle aabb to circle collisisons here
				}
				else if (entity->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID) && circleToCircleCollision(m_circleColliderBuffer.at(i), entity))
				{
					int otherRadius = static_cast<ColliderCircleComponent*>(entity->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();
					glm::vec2 distanceBetween = rectPosition->getPos() - otherPosition->getPos();
					float length = sqrt(distanceBetween.x * distanceBetween.x + distanceBetween.y * distanceBetween.y);
					float seperation = 1 - length / (radius + otherRadius);
					if (length != 0)
					{
						distanceBetween /= length;
						distanceBetween = distanceBetween * seperation * m_seperationScaler;
					}
					else
					{
						distanceBetween = glm::vec2(m_seperationScaler, m_seperationScaler);
					}
					rectPosition->addPos(distanceBetween);
					otherPosition->addPos(-distanceBetween);
				}
			}
		}
	}
	m_circleColliderBuffer.clear();
	m_quadTree.clear();
}

bool CollisionSystem::circleToCircleCollision(Entity* t_entityCircle1, Entity* t_entityCircle2)
{
	glm::vec2 position1 = static_cast<TransformComponent*>(t_entityCircle1->getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos();
	int radius1 = static_cast<ColliderCircleComponent*>(t_entityCircle1->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();
	glm::vec2 position2 = static_cast<TransformComponent*>(t_entityCircle2->getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos();
	int radius2 = static_cast<ColliderCircleComponent*>(t_entityCircle2->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();

	glm::vec2 distanceBetween = position1 - position2;
	return sqrt(distanceBetween.x * distanceBetween.x + distanceBetween.y * distanceBetween.y) < radius1 + radius2;
}

bool CollisionSystem::circleToAABBCollision(Entity* t_entityCircle, Entity* t_entityAABB)
{
	glm::vec2 circlePosition = static_cast<TransformComponent*>(t_entityCircle->getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos();
	int radius = static_cast<ColliderCircleComponent*>(t_entityCircle->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();
	glm::vec2 rectPosition = static_cast<TransformComponent*>(t_entityAABB->getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos();
	glm::vec2 bounds = static_cast<ColliderAABBComponent*>(t_entityAABB->getAllComps().at(COMPONENT_ID::COLLIDER_AABB_ID))->getBounds();

	int deltaX = circlePosition.x - std::max(rectPosition.x, std::min(circlePosition.x, rectPosition.x + bounds.x));
	int deltaY = circlePosition.y - std::max(rectPosition.y, std::min(circlePosition.y, rectPosition.y + bounds.y));
	return (deltaX * deltaX + deltaY * deltaY) < (radius * radius);
}
