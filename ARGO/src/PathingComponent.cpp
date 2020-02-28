#include "stdafx.h"
#include "PathingComponent.h"

PathingComponent::PathingComponent() :
	Component(ComponentType::Pathing),
	distance(std::numeric_limits<float>::max()),
	totalDistance(std::numeric_limits<float>::max()),
	previous(nullptr)
{
}

void PathingComponent::reset()
{
	distance = std::numeric_limits<float>::max();
	totalDistance = std::numeric_limits<float>::max();
	previous = nullptr;
}

float PathingComponent::getDistance()
{
	return distance;
}

float PathingComponent::getTotalDistance()
{
	return totalDistance;
}

Entity* PathingComponent::getPrevious()
{
	return previous;
}

void PathingComponent::setDistance(float t_value)
{
	distance = t_value;
}

void PathingComponent::setTotalDistance(float t_value)
{
	totalDistance = t_value;
}

void PathingComponent::setPrevious(Entity* t_value)
{
	previous = t_value;
}
