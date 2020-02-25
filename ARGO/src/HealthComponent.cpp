#include "stdafx.h"
#include "HealthComponent.h"

HealthComponent::HealthComponent(const int t_maxHp, int t_hp, float t_invincibilityFrames) :
	Component(ComponentType::Health),
	MAX_HEALTH(t_maxHp),
	INVINCIBILITY_FRAMES(t_invincibilityFrames),
	m_invincibilityCooldown(0),
	m_health(t_hp)
{

}

HealthComponent::~HealthComponent()
{
}

int HealthComponent::getHealth() const
{
	return m_health;
}

void HealthComponent::setHealth(int t_amount)
{
	if (t_amount > MAX_HEALTH)
	{
		m_health = MAX_HEALTH;
	}
	else
	{
		m_health = t_amount;
	}
}

void HealthComponent::addHealth(int t_amount)
{
	if (t_amount + m_health > MAX_HEALTH)
	{
		m_health = MAX_HEALTH;
	}
	else
	{
		m_health += t_amount;
	}
}

void HealthComponent::reduceHealth(int t_amount)
{
	if (m_invincibilityCooldown <=0)
	{
		m_invincibilityCooldown = INVINCIBILITY_FRAMES;
		m_health -= t_amount;
	}
}

void HealthComponent::reduceInvincibilityCooldown(float t_amount)
{
	if (m_invincibilityCooldown > 0)
	{
		m_invincibilityCooldown -= t_amount;
	}
}

bool HealthComponent::isAlive() const
{
	return m_health > 0;
}

int HealthComponent::getMaxHealth()
{
	return MAX_HEALTH;
}
