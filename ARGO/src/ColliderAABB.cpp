#include "stdafx.h"
#include "ColliderAABBComponent.h"

ColliderAABBComponent::ColliderAABBComponent(glm::vec2 t_bounds) :
	Component(ComponentType::ColliderAABB),
	m_bounds(t_bounds)
{
}

glm::vec2 ColliderAABBComponent::getBounds()
{
	return m_bounds;
}

void ColliderAABBComponent::setBounds(glm::vec2 t_bounds)
{
	m_bounds = t_bounds;
}
