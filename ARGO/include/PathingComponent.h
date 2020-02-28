#pragma once
#include "Component.h"
#include "Entity.h"
class PathingComponent :
	public Component
{
public:
	PathingComponent();
	void reset();

	float getDistance();
	float getTotalDistance();
	Entity* getPrevious();

	void setDistance(float t_value);
	void setTotalDistance(float t_value);
	void setPrevious(Entity* t_value);

private:
	float distance;
	float totalDistance;
	Entity* previous;
};

