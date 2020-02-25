#pragma once
#include <iostream>
#include "Rumble.h"
#include "Utilities.h"

/// <summary>
/// data to store the getCurrent state of the controller
/// This class is strictly for Xbox controllers
/// </summary>
struct GamePadState
{
	bool button[Utilities::NUMBER_OF_CONTROLLER_BUTTONS];
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
	RightTrigger,
	LeftTrigger,
	LeftThumbStickUp,
	LeftThumbStickDown,
	LeftThumbStickLeft,
	LeftThumbStickRight,
	RightThumbStickUp,
	RightThumbStickDown,
	RightThumbStickLeft,
	RightThumbStickRight
};

/// <summary>
/// Class for Controllers and their input
/// </summary>
class Controller
{
public:
	Controller();
	Controller(const Controller& t_otherController);
	void operator=(const Controller& t_otherController);
	~Controller();
	void initialiseController();
	void update();
	bool isConnected() const;
	int getIndex() const;
	int getCount() const;
	GamePadState getCurrent() const;
	GamePadState getPrevious() const;

	ButtonState getButtonState(ButtonType t_ButtonType);
	SDL_GameController* getSDLController();

	void activateRumble(RumbleStrength t_strength, RumbleLength t_length);
	void setRumbleStrength(float t_newStrength);
	void setRumbleTime(float t_time);
	float getRumbleStrength();
	float getRumbleTime();

private:
	// dead zone used by thumb sticks
	const int THUMB_STICK_THRESHOLD = 8000;

	//dead zone used for the thumbsticks basic movement
	const int THUMB_STICK_BUTTON_THRESHOLD = 32000;

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