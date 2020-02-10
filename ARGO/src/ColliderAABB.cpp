#include "stdafx.h"
#include "ColliderAABB.h"

ColliderAABB::ColliderAABB(glm::vec2 t_bounds) :
	Component(ComponentType::ColliderAABB),
	m_bounds(t_bounds)
{
}

glm::vec2 ColliderAABB::getBounds()
{
	return m_bounds;
}

void ColliderAABB::setBounds(glm::vec2 t_bounds)
{
	m_bounds = t_bounds;
}
