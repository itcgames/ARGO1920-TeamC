#include "stdafx.h"
#include "FireRateComponent.h"

FireRateComponent::FireRateComponent(Uint32 t_delay) :
	Component(ComponentType::FireRate),
	m_delay(t_delay),
	m_lastFire(0)
{
}

Uint32 FireRateComponent::getNextFire()
{
	return m_lastFire + m_delay;
}

void FireRateComponent::setLastFire(Uint32 t_lastFire)
{
	m_lastFire = t_lastFire;
}
