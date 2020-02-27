#pragma once
#include "Component.h"

enum class Weapon
{
	Pistol,
	MachineGun,
	GrenadeLauncher,
	Shotgun
};

class WeaponComponent :
	public Component
{
public:
	WeaponComponent();
	void reduceCooldowns(float t_dt);
	bool fireGun();
	bool throwGlowStick();
	Weapon getCurrent();
	int getMaxAmmo();
	int getAmmo();
	void setCurrent(Weapon t_weapon);
	void fillAmmo(Weapon t_type);


private:
	float m_gunCooldown;
	float m_glowStickCooldown;
	Weapon m_currentWeapon;
	int m_ammo;

	const float GLOWSTICK_COOLDOWN = 120;
	const float PISTOL_COOLDOWN = 20;
	const float MACHINEGUN_COOLDOWN = 10;
	const float GREANADE_LAUNCHER_COOLDOWN = 30;
	const float SHOTGUN_COOLDOWN = 20;

	const int MACHINEGUN_MAX_AMMO = 60;
	const int SHOTGUN_MAX_AMMO = 30;
	const int GRENADE_LAUNCHER_MAX_AMMO = 10;
};

