#include "stdafx.h"
#include "FlowFieldComponent.h"

FlowFieldComponent::FlowFieldComponent() :
	Component(ComponentType::FlowField),
	m_closestNeightbour(nullptr),
	m_weight(std::numeric_limits<int>::max())
{
}

Entity* FlowFieldComponent::getClosestNeightbour()
{
	return m_closestNeightbour;
}

void FlowFieldComponent::setClosestNeighbour(Entity* t_closestNeightbour)
{
	m_closestNeightbour = t_closestNeightbour;
}

void FlowFieldComponent::setWeight(int t_weight)
{
	m_weight = t_weight;
}

int FlowFieldComponent::getWeight()
{
	return m_weight;
}

void FlowFieldComponent::reset()
{
	m_closestNeightbour = nullptr;
	m_weight = std::numeric_limits<int>::max();
}
