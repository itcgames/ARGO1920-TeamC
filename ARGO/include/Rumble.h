#pragma once

/// <summary>
/// List of the different set Strengths the rumble can have
/// </summary>
enum class RumbleStrength
{
	Strong,
	Medium,
	Weak,
	// Default is for when a specific value is used
	Default
};

/// <summary>
/// List of the different length of time the rumble can have
/// </summary>
enum class RumbleLength
{
	Long,
	Medium,
	Short,
	//Default is for when a specific value is used
	Default
};

/// <summary>
/// Class That handles the rumble of a controller
/// </summary>
class Rumble
{
public:
	Rumble();
	Rumble(const Rumble& t_otherRumble);
	void operator=(const Rumble& t_otherRumble);
	void init(SDL_GameController* t_controller);
	void activateRumble();
	void activateRumble(RumbleStrength t_setStrength, RumbleLength t_setLength);
	float getTime();
	float getStrength();
	void setTime(float t_newTime);
	void setTime(RumbleLength t_newTime);
	void setStrength(float t_newStrength);
	void setStrength(RumbleStrength t_newStrength);
private:
	// Controller that effect is applied on
	SDL_GameController* m_controller;
	// Haptic that causes effect
	SDL_Haptic* m_haptic;
	// Length of time the rumble plays for in milliseconds
	float m_time;
	// Strength of the rumble - From 0 to 1
	float m_strength;

	// const for the different values of RumbleLength for the time the rumble will activate for
	const int LONG_TIME = 750;
	const int MEDIUM_TIME = 500;
	const int SHORT_TIME = 150;

	// const for the different values of RumbleStrength for the strength the rumble will be
	const float STRONG_STRENGTH = 1.0f;
	const float MEDIUM_STRENGTH = 0.5f;
	const float WEAK_STRENGTH = 0.25f;
};

