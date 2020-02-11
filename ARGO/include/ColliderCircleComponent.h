#pragma once
#include "Component.h"
class ColliderCircleComponent : public Component
{
public:
	ColliderCircleComponent(int t_radius);
	int getRadius();
	void setRadius(int t_radius);
private:
	int m_radius;
};

