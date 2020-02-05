#include "stdafx.h"
#include "HealthComponent.h"

HealthComponent::HealthComponent(const int t_maxHp, int t_hp) :
	Component(ComponentType::Health),
	MAX_HEALTH(t_maxHp),
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
	m_health -= t_amount;
}

bool HealthComponent::isAlive() const
{
	return m_health > 0;
}
