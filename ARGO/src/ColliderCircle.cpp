#include "stdafx.h"
#include "ColliderCircle.h"

ColliderCircle::ColliderCircle(int t_radius) :
	Component(ComponentType::ColliderCircle),
	m_radius(t_radius)
{
}

int ColliderCircle::getRadius()
{
	return m_radius;
}

void ColliderCircle::setRadius(int t_radius)
{
	m_radius = t_radius;
}
