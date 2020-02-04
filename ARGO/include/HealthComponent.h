#pragma once
#include "Component.h"
class HealthComponent : public Component
{
public:
	HealthComponent(int t_hp = 10);
	~HealthComponent();

	int getHealth() { return m_health; }
	void setHealth(int t_amount) { m_health = t_amount; }
	void addHealth(int t_amount) { m_health += t_amount; }
	void reduceHealth(int t_amount) { m_health -= t_amount; }


private:
	int m_health;
};


