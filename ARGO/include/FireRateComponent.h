#pragma once
#include "Component.h"
class FireRateComponent : public Component
{
public:
	FireRateComponent(Uint32 t_delay);
	Uint32 getNextFire();
	void setLastFire(Uint32 t_lastFire);
private:
	Uint32 m_lastFire;
	Uint32 m_delay;
};

