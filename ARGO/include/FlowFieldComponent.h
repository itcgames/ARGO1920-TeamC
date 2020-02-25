#pragma once
#include "Component.h"
#include <Entity.h>
class FlowFieldComponent : public Component
{
public:
	FlowFieldComponent();
	Entity* getClosestNeightbour();
	void setClosestNeighbour(Entity* t_closestNeightbour);
	void setWeight(int t_weight);
	int getWeight();
	void reset();

private:
	Entity* m_closestNeightbour;
	int m_weight;
};

