#include "stdafx.h"
#include "lightFieldComponent.h"

lightFieldComponent::lightFieldComponent() :
	Component(ComponentType::LightField),
	m_weight(0)
{
}

int lightFieldComponent::getWeight()
{
	return m_weight;
}

void lightFieldComponent::setWeight(int t_weight)
{
	m_weight = t_weight;
}

void lightFieldComponent::reset()
{
	m_weight = 255;
}
