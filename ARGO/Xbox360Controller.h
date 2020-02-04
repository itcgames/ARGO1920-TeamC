//#ifndef XBOX360CONTROLLER
//#define XBOX360CONTROLLER
//
//#include <iostream>
//
///// <summary>
///// data to store the getCurrent state of the controller
///// This class is strictly for Xbox controllers
///// </summary>
//struct GamePadState
//{
//	bool A{ false };
//	bool B{ false };
//	bool X{ false };
//	bool Y{ false };
//	bool LB{ false };
//	bool RB{ false };
//	bool LeftThumbStickClick{ false };
//	bool RightThumbStickClick{ false };
//	bool DpadUp{ false };
//	bool DpadDown{ false };
//	bool DpadLeft{ false };
//	bool DpadRight{ false };
//	bool Start{ false };
//	bool Back{ false };
//	bool Xbox{ false };
//	float RTrigger{ 0 };
//	float LTrigger{ 0 };
//	glm::vec2 RightThumbStick{ 0.0f, 0.0f };
//	glm::vec2 LeftThumbStick{ 0.0f, 0.0f };
//};
//
//class Xbox360Controller
//{
//public:
//	Xbox360Controller();
//	~Xbox360Controller();
//	void update();
//	bool isConnected() const;
//	int getIndex() const;
//	int getCount() const;
//	GamePadState getCurrent() const;
//	GamePadState getPrevious() const;
//
//private:
//	// dead zone for the dpad? (works like an another joystick)
//	const int DPAD_THRESHOLD = 50;
//	// index of getCurrent controller
//	int m_joystickIndex;
//	// static int to differentiate between several connected controllers
//	static int S_CONTROLLER_COUNT;
//	// getCurrent state of all the buttons
//	GamePadState m_current;
//	// getPrevious state used to check for button presses and holds
//	GamePadState m_previous;
//};
//
//#endif // !XBOX360CONTROLLER