#include "stdafx.h"
#include "Controller.h"

int Controller::S_CONTROLLER_COUNT = -1;

Controller::Controller()
{
	m_controller = NULL;
	int test = SDL_NumJoysticks();

	for (int index = (S_CONTROLLER_COUNT + 1); index < SDL_NumJoysticks(); index++)
	{
		// checks if connected controller is supported by SDL
		if (SDL_IsGameController(index))
		{
			// opens controller for it to be used
			m_controller = SDL_GameControllerOpen(index);

			Controller::S_CONTROLLER_COUNT++;
			m_controllerIndex = S_CONTROLLER_COUNT;
			std::string name((SDL_GameControllerName(m_controller)));
			controllerName = name;
			break;
		}
	}

	if (m_controller == NULL)
	{
		std::cout << "Couldn't Open Game Controller" << std::endl;;
	}
	else
	{
		// initalise buttons to false
		for (int index = 0; index < 14; index++)
		{
			m_current.button[index] = false;
			m_previous.button[index] = false;
		}
	}
}

Controller::~Controller()
{
	SDL_GameControllerClose(m_controller);
} 

/// <summary>
/// Updates the states of the gamePadState variables from the current state of the controller
/// </summary>
void Controller::update()
{
	//Update the last current values to be previous
	m_previous = m_current;
	SDL_GameControllerUpdate();

	// Update buttons with current value if button pressed 
	m_current.button[(int)ButtonType::A] = SDL_GameControllerGetButton(m_controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A);
	m_current.button[(int)ButtonType::B] = SDL_GameControllerGetButton(m_controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B);
	m_current.button[(int)ButtonType::X] = SDL_GameControllerGetButton(m_controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X);
	m_current.button[(int)ButtonType::Y] = SDL_GameControllerGetButton(m_controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_Y);
	m_current.button[(int)ButtonType::Start] = SDL_GameControllerGetButton(m_controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START);
	m_current.button[(int)ButtonType::Back] = SDL_GameControllerGetButton(m_controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_BACK);
	m_current.button[(int)ButtonType::LB] = SDL_GameControllerGetButton(m_controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
	m_current.button[(int)ButtonType::RB] = SDL_GameControllerGetButton(m_controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
	m_current.button[(int)ButtonType::LeftThumbStickClick] = SDL_GameControllerGetButton(m_controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSTICK);
	m_current.button[(int)ButtonType::RightThumbStickClick] = SDL_GameControllerGetButton(m_controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSTICK);
	m_current.button[(int)ButtonType::DpadUp] = SDL_GameControllerGetButton(m_controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP);
	m_current.button[(int)ButtonType::DpadDown] = SDL_GameControllerGetButton(m_controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	m_current.button[(int)ButtonType::DpadLeft] = SDL_GameControllerGetButton(m_controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	m_current.button[(int)ButtonType::DpadRight] = SDL_GameControllerGetButton(m_controller, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

	//Updates the values of the triggers for how much they are pressed
	m_current.RTrigger = SDL_GameControllerGetAxis(m_controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
	m_current.LTrigger = SDL_GameControllerGetAxis(m_controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERLEFT);

	// Update position values of the left and right thumb sticks - Set positions to 0 if value is under threshold
	m_current.LeftThumbStick = glm::vec2(SDL_GameControllerGetAxis(m_controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX),
										 SDL_GameControllerGetAxis(m_controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY));
	if (abs(m_current.LeftThumbStick.x) < THUMB_STICK_THRESHOLD && abs(m_current.LeftThumbStick.y) < THUMB_STICK_THRESHOLD)
	{
		m_current.LeftThumbStick = glm::vec2(0.0f, 0.0f);
	}
	m_current.RightThumbStick = glm::vec2(SDL_GameControllerGetAxis(m_controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX),
										  SDL_GameControllerGetAxis(m_controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTY));
	if (abs(m_current.RightThumbStick.y) < THUMB_STICK_THRESHOLD && abs(m_current.RightThumbStick.y) < THUMB_STICK_THRESHOLD)
	{
		m_current.RightThumbStick = glm::vec2(0.0f, 0.0f);
	}
}

/// <summary>
/// Checks if controller is connected and opened in SDL
/// </summary>
/// <returns>true if connected and opened, else false</returns>
bool Controller::isConnected() const
{
	return SDL_GameControllerGetAttached(m_controller);
}

int Controller::getIndex() const
{
	return m_controllerIndex;
}

int Controller::getCount() const
{
	return Controller::S_CONTROLLER_COUNT;
}

GamePadState Controller::getCurrent() const
{
	return m_current;
}

GamePadState Controller::getPrevious() const
{
	return m_previous;
}

/// <summary>
/// Get the current state of the corresponding button on the controller
/// State can be either pressed, held, released or not held
/// </summary>
/// <param name="t_buttonType">type of the button being checked</param>
/// <returns>state of the button</returns>
ButtonState Controller::getButtonState(ButtonType t_buttonType)
{
	ButtonState currentState = ButtonState::NotPressed;
	bool currentPressed = m_current.button[(int)t_buttonType];
	bool previousPressed = m_previous.button[(int)t_buttonType];

	if (currentPressed && previousPressed) currentState = ButtonState::Held; 
	else if (currentPressed && !previousPressed) currentState = ButtonState::Pressed;
 	else if (!currentPressed && previousPressed) currentState = ButtonState::Released;

 #ifdef _DEBUG
	if (ButtonState::NotPressed != currentState)
	{
		std::string debugString = "Controller " + std::to_string(m_controllerIndex) + " " + controllerName + " " + getButtonName(t_buttonType);
		if (ButtonState::Pressed == currentState)  debugString += "Pressed";
		else if (ButtonState::Held == currentState) debugString += "Held";
		else if (ButtonState::Released == currentState) debugString += "Released";
		std::cout << debugString << std::endl;
	}
#endif // !_DEBUG
	return currentState; 
}

/// <summary>
/// Get the current state of the corresponding item on controller that has a axis value
/// States can be either moved or not moved
/// </summary>
/// <param name="t_axisType">type of the axis being checked</param>
/// <returns>State of the axis</returns>
AxisState Controller::getAxisState(AxisType t_axisType)
{
#ifdef _DEBUG
	std::string debugString = "Controller " + std::to_string(m_controllerIndex) + " " + controllerName + " \n";
#endif // _DEBUG
	AxisState currentState = AxisState::NotMoved;

	if (AxisType::LeftThumbStick == t_axisType && m_current.LeftThumbStick != m_previous.LeftThumbStick)
	{
#ifdef _DEBUG 
		debugString += "Left Thumb Stick Moved, Position: X: " + std::to_string(m_current.LeftThumbStick.x) + " ,Y: " + std::to_string(m_current.LeftThumbStick.y);
#endif // _DEBUG
		currentState = AxisState::Moved;
	} 
	else if (AxisType::RightThumbStick == t_axisType && m_current.RightThumbStick != m_previous.RightThumbStick)
	{
#ifdef _DEBUG
		debugString += "Right Thumb Stick Moved, Position: X: " + std::to_string(m_current.RightThumbStick.x) + " ,Y: " + std::to_string(m_current.RightThumbStick.y);
#endif // _DEBUG
		currentState = AxisState::Moved;
	}
	else if (AxisType::LeftTrigger == t_axisType && m_current.LTrigger != m_previous.LTrigger)
	{
#ifdef _DEBUG
		debugString += "Left Trigger Moved, Value: " + std::to_string(m_current.LTrigger);
#endif // _DEBUG
		currentState = AxisState::Moved;
	}
	else if(AxisType::RightTrigger == t_axisType && m_current.RTrigger != m_previous.RTrigger)
	{
#ifdef _DEBUG
		debugString += "Right Trigger Moved, Value: " + std::to_string(m_current.RTrigger);
#endif // _DEBUG
		currentState = AxisState::Moved;
	}

#ifdef _DEBUG
	if (AxisState::NotMoved != currentState)
	{
		std::cout << debugString << std::endl;
	}
#endif // _DEBUG

	return currentState;
}

/// <summary>
/// Returns the name of the button that's type is passed to the function
/// </summary>
/// <param name="t_buttonType">type of button you want the name of</param>
/// <returns>name of button</returns>
std::string Controller::getButtonName(ButtonType t_buttonType)
{
	switch (t_buttonType)
	{
	case ButtonType::A:
		return "A";
	case ButtonType::B:
		return "B";
	case ButtonType::X:
		return "X";
	case ButtonType::Y:
		return "Y";
	case ButtonType::RB:
		return "RB";
	case ButtonType::LB:
		return "LB";
	case ButtonType::LeftThumbStickClick:
		return "LEFT STICK CLICK";
	case ButtonType::RightThumbStickClick:
		return "RIGHT STICK CLICK";
	case ButtonType::DpadUp:
		return "DPAD UP";
	case ButtonType::DpadDown:
		return "DPAD DOWN";
	case ButtonType::DpadRight:
		return "DPAD RIGHT";
	case ButtonType::DpadLeft:
		return "DPAD LEFT";
	case ButtonType::Start:
		return "START";
	case ButtonType::Back:
		return "BACK";
	default:
		return "";
	}
}
