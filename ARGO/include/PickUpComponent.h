#pragma once
#include "Component.h"

enum class PickupType
{
	Health,
	MachineGun,
	Grenade,
	Shotgun
};

class PickUpComponent :
	public Component
{
public:
	PickUpComponent(PickupType t_type = PickupType::Health, bool t_rotating = true, float t_rotationSpeed = 0.1f, float t_healthChange = 1);
	void setRotationSpeed(float t_speed);
	float getRotationSpeed();
	void setRotating(bool t_rotating);
	float getRotating();
	void setPickupType(PickupType t_type);
	PickupType getPickupType();
	void setHealthChange(float t_health);
	float getHealthChange();
private:
	float m_rotationSpeed;
	bool m_rotating;
	//0 is Unset. 1 Is Ammo, 2 is Health.
	PickupType m_pickUpType;
	//Value between 1 and 0. Percentage of health restored.
	float m_healthChange;
};

