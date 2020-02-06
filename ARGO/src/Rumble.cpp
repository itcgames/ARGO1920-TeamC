#include "stdafx.h"
#include "Rumble.h"

Rumble::Rumble( ):
	m_strength{ 0.0f },
	m_time{ 0.0f }
{	
}

void Rumble::init(SDL_GameController* t_controller)
{
	m_controller = t_controller;
	try
	{
		if (!SDL_INIT_HAPTIC) SDL_INIT_HAPTIC;
		// Open haptic for the device
		m_haptic = SDL_HapticOpenFromJoystick(SDL_GameControllerGetJoystick(m_controller));
		if (!m_haptic) throw "Error Opening Haptic";
		SDL_HapticRumbleInit(m_haptic);
	}
	catch (std::string error)
	{
		std::cout << error << std::endl;
	}
}

/// <summary>
/// Plays Rumble effect based on the current value for time and strength
/// </summary>
void Rumble::activateRumble()
{
	SDL_HapticRumblePlay(m_haptic, m_strength, m_time);
}

/// <summary>
/// Overloaded version of function that allows one to determine strength and time
/// of the rumble based on the enums
/// </summary>
/// <param name="t_strengthType">Strength of Rumble</param>
/// <param name="t_lengthType">Length of time for the Rumble</param>
void Rumble::activateRumble(RumbleStrength t_strengthType, RumbleLength t_lengthType)
{
	setStrength(t_strengthType);
	setTime(t_lengthType);
	activateRumble();
}

float Rumble::getTime()
{
	return m_time;
}

float Rumble::getStrength()
{
	return m_strength;
}

void Rumble::setTime(float t_newTime)
{
	// clamps time so that it can't be lower than 0
	m_time = glm::clamp(t_newTime, 0.0f, std::numeric_limits<float>().max());
}

/// <summary>
/// Sets the time variable based on enum passed in
/// </summary>
/// <param name="t_lengthType">Length of time enum for the rumble</param>
void Rumble::setTime(RumbleLength t_lengthType)
{
	switch (t_lengthType)
	{
	case RumbleLength::Long:
		m_time = 750;
		break;
	case RumbleLength::Medium:
		m_time = 500;
		break;
	case RumbleLength::Short:
		m_time = 150;
		break;
	default:
		break;
	}
}

void Rumble::setStrength(float t_newStrength)
{
	// clamps strength so that its between 0 and 1
	m_strength = glm::clamp(t_newStrength, 0.0f, 1.0f);
}

/// <summary>
/// Sets the strength variable based on enum passed in
/// </summary>
/// <param name="t_strengthType">Strength enum for the rumble</param>
void Rumble::setStrength(RumbleStrength t_strengthType)
{
	switch (t_strengthType)
	{
	case RumbleStrength::Strong:
		m_strength = 1.0f;
		break;
	case RumbleStrength::Medium:
		m_strength = 0.5f;
		break;
	case RumbleStrength::Weak:
		m_strength = 0.25f;
		break; 
	default:
		break;
	}
}
