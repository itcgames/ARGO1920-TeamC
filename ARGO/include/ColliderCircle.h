#pragma once
#include "Component.h"
class ColliderCircle : public Component
{
public:
	ColliderCircle(int t_radius);
	int getRadius();
	void setRadius(int t_radius);
private:
	int m_radius;
};

