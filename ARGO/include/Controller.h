#pragma once

#include <iostream>
#include "Rumble.h"

/// <summary>
/// data to store the getCurrent state of the controller
/// This class is strictly for Xbox controllers
/// </summary>
struct GamePadState
{
	bool button[14]; 
	float RTrigger{ 0 };
	float LTrigger{ 0 };
	glm::vec2 RightThumbStick{ 0.0f, 0.0f };
	glm::vec2 LeftThumbStick{ 0.0f, 0.0f };
};

/// <summary>
/// The Different States the button can be on based on the
/// values of current and previous gamepadstate
/// </summary>
enum class ButtonState
{
	Pressed,
	Held,
	Released,
	NotPressed
};

/// <summary>
/// The different states the axis can be similar to the button state above
/// </summary>
enum class AxisState
{
	Moved,
	NotMoved
};

/// <summary>
/// The different non buttons on the controller that gets its value from SDL axis
/// </summary>
enum class AxisType
{
	RightThumbStick,
	LeftThumbStick,
	LeftTrigger,
	RightTrigger,
	// used to get size of enum
	Count
};

/// <summary>
/// The different buttons present on the controller gotten from the SDL button value
/// </summary>
enum class ButtonType
{
	A,
	B,
	X,
	Y,
	RB,
	LB,
	LeftThumbStickClick,
	RightThumbStickClick,
	DpadUp,
	DpadDown,
	DpadRight,
	DpadLeft,
	Start,
	Back,
	// used to get size of enum
	Count
};

/// <summary>
/// Class for Controllers and their input
/// </summary>
class Controller
{
public:
	Controller();
	~Controller();
	void update();
	bool isConnected() const;
	int getIndex() const;
	int getCount() const;
	GamePadState getCurrent() const;
	GamePadState getPrevious() const;

	ButtonState getButtonState(ButtonType t_ButtonType);
	AxisState getAxisState(AxisType t_AxisType);
	SDL_GameController* getSDLController();

	void activateRumble(RumbleStrength t_strength, RumbleLength t_length);
	void setRumbleStrength(float t_newStrength);
	void setRumbleTime(float t_time);
	float getRumbleStrength();
	float getRumbleTime();

private:
	// dead zone for the dpad? (works like an another joystick)
	const int THUMB_STICK_THRESHOLD = 3200;
	// index of getCurrent controller
	int m_controllerIndex;
	// static int to differentiate between several connected controllers
	static int S_CONTROLLER_COUNT;
	// getCurrent state of all the buttons
	GamePadState m_current;
	// getPrevious state used to check for button presses and holds
	GamePadState m_previous;

	SDL_GameController* m_controller;

	// name of controller gotten by calling SDL_GameControllerName 
	std::string m_controllerName;

	const std::string XBOX_CONTROLLER_NAME = "XInput Controller";
	const std::string SWITCH_CONTROLLER_NAME = "Nintendo Switch Pro Controller";

	std::string getButtonName(ButtonType t_ButtonType);

	Rumble m_rumble;
};