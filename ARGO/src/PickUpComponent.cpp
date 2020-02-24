#include "stdafx.h"
#include "PickUpComponent.h"

PickUpComponent::PickUpComponent(int t_type, bool t_rotating, float t_rotationSpeed, float t_healthChange, float t_ammoChange):
	Component(ComponentType::PickUp),
	m_pickUpType(t_type),
	m_rotating(t_rotating),
	m_rotationSpeed(t_rotationSpeed),
	m_healthChange(t_healthChange),
	m_ammoChange(t_ammoChange)
{
}

void PickUpComponent::setRotationSpeed(float t_speed)
{
	m_rotationSpeed = t_speed;
}

float PickUpComponent::getRotationSpeed()
{
	return m_rotationSpeed;
}

void PickUpComponent::setRotating(bool t_rotating)
{
	m_rotating = t_rotating;
}

float PickUpComponent::getRotating()
{
	return m_rotating;
}

void PickUpComponent::setPickupType(int t_type)
{
	m_pickUpType = t_type;
}

int PickUpComponent::getPickupType()
{
	return m_pickUpType;
}

void PickUpComponent::setHealthChange(float t_health)
{
	m_healthChange = t_health;
}

float PickUpComponent::getHealthChange()
{
	return m_healthChange;
}

void PickUpComponent::setAmmoChange(float t_ammo)
{
	m_ammoChange = t_ammo;
}

float PickUpComponent::getAmmmoChange()
{
	return m_ammoChange;
}
