#pragma once
#include "Component.h"

class HealthComponent : public Component
{
public:
	HealthComponent(const int t_maxHp, int t_hp);
	~HealthComponent();

	int getHealth() const;
	bool isAlive() const;

	void setHealth(int t_amount);
	void addHealth(int t_amount);
	void reduceHealth(int t_amount);

private:
	int m_health;
	const int MAX_HEALTH;
};


