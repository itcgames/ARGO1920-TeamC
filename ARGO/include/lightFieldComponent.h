#pragma once
#include "Component.h"
class lightFieldComponent :
	public Component
{
public:
	lightFieldComponent();
	int getWeight();
	void setWeight(int t_weight);
	void reset();

private:
	int m_weight;
};

