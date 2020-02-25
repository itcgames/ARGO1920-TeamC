#pragma once
#include "Component.h"

class HealthComponent : public Component
{
public:
	HealthComponent(const int t_maxHp, int t_hp, float t_invincibilityFrames = 0.0f);
	~HealthComponent();

	int getHealth() const;
	bool isAlive() const;

	int getMaxHealth();

	void setHealth(int t_amount);
	void addHealth(int t_amount);
	void resetHealth();
	bool reduceHealth(int t_amount);
	void reduceInvincibilityCooldown(float t_amount);

private:
	int m_health;
	float m_invincibilityCooldown;
	const float INVINCIBILITY_FRAMES;
	const int MAX_HEALTH;
};


