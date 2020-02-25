#pragma once
#include "Component.h"
class PickUpComponent :
	public Component
{
public:
	PickUpComponent(int t_type, bool t_rotating = true, float t_rotationSpeed = 0.1f, float t_healthChange = 1, float t_ammoChange = 1);
	void setRotationSpeed(float t_speed);
	float getRotationSpeed();
	void setRotating(bool t_rotating);
	float getRotating();
	void setPickupType(int t_type);
	int getPickupType();
	void setHealthChange(float t_health);
	float getHealthChange();
	void setAmmoChange(float t_ammo);
	float getAmmmoChange();
private:
	float m_rotationSpeed{ 0.1f };
	bool m_rotating{ true };
	//0 is Unset. 1 Is Ammo, 2 is Health.
	int m_pickUpType{ 0 };
	//Value between 1 and 0. Percentage of health restored.
	float m_healthChange{ 1 };
	//Value between 1 and 0. Percentage of ammo restored.
	float m_ammoChange{ 1 };


};

