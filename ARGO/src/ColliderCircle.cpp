#include "stdafx.h"
#include "ColliderCircleComponent.h"

ColliderCircleComponent::ColliderCircleComponent(int t_radius) :
	Component(ComponentType::ColliderCircle),
	m_radius(t_radius)
{
}

int ColliderCircleComponent::getRadius()
{
	return m_radius;
}

void ColliderCircleComponent::setRadius(int t_radius)
{
	m_radius = t_radius;
}
