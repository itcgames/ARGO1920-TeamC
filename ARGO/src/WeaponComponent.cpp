#include "stdafx.h"
#include "WeaponComponent.h"

WeaponComponent::WeaponComponent() :
	Component(ComponentType::Weapon),
	m_currentWeapon(Weapon::Pistol),
	m_glowStickCooldown(0.0f),
	m_gunCooldown(0.0f)
{
}

void WeaponComponent::reduceCooldowns(float t_dt)
{
	if (m_glowStickCooldown > 0.0f)
	{
		m_glowStickCooldown -= t_dt;
	}
	if (m_gunCooldown > 0.0f)
	{
		m_gunCooldown -= t_dt;
	}
}

bool WeaponComponent::fireGun()
{
	if (m_gunCooldown <= 0.0f)
	{
		switch (m_currentWeapon)
		{
		case Weapon::Pistol:
			m_gunCooldown = PISTOL_COOLDOWN;
			break;
		case Weapon::MachineGun:
			m_gunCooldown = MACHINEGUN_COOLDOWN;
			break;
		case Weapon::GrenadeLauncher:
			m_gunCooldown = GREANADE_LAUNCHER_COOLDOWN;
			break;
		default:
			break;
		}

		if (m_currentWeapon != Weapon::Pistol)
		{
			m_ammo--;
			if (m_ammo <= 0)
			{
				m_currentWeapon = Weapon::Pistol;
			}
		}
		return true;
	}
	return false;
}

bool WeaponComponent::throwGlowStick()
{
	if (m_glowStickCooldown <= 0.0f)
	{
		m_glowStickCooldown = GLOWSTICK_COOLDOWN;
		return true;
	}
	return false;
}

Weapon WeaponComponent::getCurrent()
{
	return m_currentWeapon;
}

void WeaponComponent::setCurrent(Weapon t_weapon)
{
	m_currentWeapon = t_weapon;
}