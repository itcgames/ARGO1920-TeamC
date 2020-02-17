#include "stdafx.h"
#include "FireRateComponent.h"

FireRateComponent::FireRateComponent(Uint16 t_delay) :
	Component(ComponentType::FireRate),
	m_delay(t_delay),
	m_lastFire(0)
{
}

Uint16 FireRateComponent::getNextFire()
{
	return m_lastFire + m_delay;
}

void FireRateComponent::setLastFire(Uint16 t_lastFire)
{
	m_lastFire = t_lastFire;
}
