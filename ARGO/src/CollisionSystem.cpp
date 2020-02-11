#include "stdafx.h"
#include "CollisionSystem.h"

CollisionSystem::CollisionSystem() :
	m_quadTree(0, glm::vec2(0, 0), glm::vec2(Utilities::LEVEL_TILE_WIDTH * Utilities::TILE_SIZE, Utilities::LEVEL_TILE_HEIGHT * Utilities::TILE_SIZE)),
	m_seperationScaler(30)
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
		glm::vec2 rectPosition = static_cast<TransformComponent*>(t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos();
		glm::vec2 bounds = static_cast<ColliderAABB*>(t_entity.getAllComps().at(COMPONENT_ID::COLLIDER_AABB_ID))->getBounds();
		Quad quad{ &t_entity, rectPosition, bounds };
		m_quadTree.insert(quad);
	}
	else if (t_entity.getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID) &&
		t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))
	{
		m_circleColliderBuffer.push_back(&t_entity);
		glm::vec2 rectPosition = static_cast<TransformComponent*>(t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos();
		int radius = static_cast<ColliderCircle*>(t_entity.getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();
		glm::vec2 bounds{ radius, radius };
		Quad quad{ &t_entity, rectPosition - bounds, bounds * 2.0f };
		m_quadTree.insert(quad);
	}
}

void CollisionSystem::handleCollisions()
{
	int count = 0;
	for (int i = 0; i < m_circleColliderBuffer.size(); i++)
	{
		TransformComponent* rectPosition = static_cast<TransformComponent*>(m_circleColliderBuffer[i]->getAllComps().at(COMPONENT_ID::TRANSFORM_ID));
		int radius = static_cast<ColliderCircle*>(m_circleColliderBuffer[i]->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();
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
					int otherRadius = static_cast<ColliderCircle*>(entity->getAllComps().at(COMPONENT_ID::COLLIDER_CIRCLE_ID))->getRadius();
					count++;
					glm::vec2 between = rectPosition->getPos() - otherPosition->getPos();
					float length = sqrt(between.x * between.x + between.y * between.y);
					float seperation = 1 - length / (radius + otherRadius);
					if (length != 0)
					{
						between /= length;
						between = between * seperation * m_seperationScaler;
					}
					else
					{
						between = glm::vec2(m_seperationScaler, m_seperationScaler);
					}
					rectPosition->addPos(between);
					otherPosition->addPos(-between);
				}
			}
		}
	}
	m_circleColliderBuffer.clear();
	std::cout << m_quadTree.countAll(0) << std::endl;
	std::cout << count << std::endl;
	m_quadTree.clear();
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

	int DeltaX = circlePosition.x - std::max(rectPosition.x, std::min(circlePosition.x, rectPosition.x + bounds.x));
	int DeltaY = circlePosition.y - std::max(rectPosition.y, std::min(circlePosition.y, rectPosition.y + bounds.y));
	return (DeltaX * DeltaX + DeltaY * DeltaY) < (radius * radius);
}
