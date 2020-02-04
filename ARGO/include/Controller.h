#ifndef CONTROLLER
#define CONTROLLER

#include <iostream>

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


enum class ButtonState
{
	Pressed,
	Held,
	Released,
	NotPressed
};

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
	Back
};

class Controller
{
public:
	Controller();
	~Controller();
	void init();
	void update();
	bool isConnected() const;
	int getIndex() const;
	int getCount() const;
	GamePadState getCurrent() const;
	GamePadState getPrevious() const;


	ButtonState getButtonState(ButtonType t_type);
private:
	// dead zone for the dpad? (works like an another joystick)
	const int DPAD_THRESHOLD = 50;
	// index of getCurrent controller
	int m_controllerIndex;
	// static int to differentiate between several connected controllers
	static int S_CONTROLLER_COUNT;
	// getCurrent state of all the buttons
	GamePadState m_current;
	// getPrevious state used to check for button presses and holds
	GamePadState m_previous;

	SDL_GameController* m_controller;

	std::string getButtonName(ButtonType t_type);
};

#endif // !CONTROLLER