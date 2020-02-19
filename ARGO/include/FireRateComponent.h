#pragma once
#include "Component.h"
class FireRateComponent : public Component
{
public:
	FireRateComponent(Uint16 t_delay);
	Uint16 getNextFire();
	void setLastFire(Uint16 t_lastFire);
private:
	Uint16 m_lastFire;
	Uint16 m_delay;
};

