#include "stdafx.h"
#include "Controller.h"

int Controller::S_CONTROLLER_COUNT = -1;

Controller::Controller()
{

	m_controller = NULL;
	int test = SDL_NumJoysticks();
	for (int index = 0; index < SDL_NumJoysticks(); index++)
	{
		// checks if connected controller is supported by SDL
		if (SDL_IsGameController(index))
		{
			// opens controller for it to be used
			m_controller = SDL_GameControllerOpen(index);
			Controller::S_CONTROLLER_COUNT++;
			m_controllerIndex = S_CONTROLLER_COUNT;
			std::string name((SDL_GameControllerName(m_controller)));
			std::cout << name << std::endl;
		}
	}
	if (m_controller == NULL)
	{
		std::cout << "Couldn't Open Game Controller" << std::endl;;
	}
	else
	{
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

void Controller::init()
{
	m_controller = NULL;
	int test = SDL_NumJoysticks();
	for (int index = 0; index < SDL_NumJoysticks(); index++)
	{
		// checks if connected controller is supported by SDL
		if (SDL_IsGameController(index))
		{
			// opens controller for it to be used
			m_controller = SDL_GameControllerOpen(index);
			Controller::S_CONTROLLER_COUNT++;
			m_controllerIndex = S_CONTROLLER_COUNT;
			std::string name((SDL_GameControllerName(m_controller)));
			std::cout << name << std::endl;
		}
	}
	if (m_controller == NULL)
	{
		std::cout << "Couldn't Open Game Controller" << std::endl;;
	}
	else
	{
		for (int index = 0; index < 14; index++)
		{
			m_current.button[index] = false;
			m_previous.button[index] = false;
		}
	}
}

void Controller::update()
{
	m_previous = m_current;
	SDL_GameControllerUpdate();


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
	
	m_current.RTrigger = SDL_GameControllerGetAxis(m_controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
	m_current.LTrigger = SDL_GameControllerGetAxis(m_controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_TRIGGERLEFT);

	m_current.LeftThumbStick = glm::vec2(SDL_GameControllerGetAxis(m_controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX),
										 SDL_GameControllerGetAxis(m_controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY));

	m_current.RightThumbStick = glm::vec2(SDL_GameControllerGetAxis(m_controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX),
										  SDL_GameControllerGetAxis(m_controller, SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTY));
}

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

ButtonState Controller::getButtonState(ButtonType t_type)
{
	bool currentPressed = m_current.button[(int)t_type];
	bool previousPressed = m_previous.button[(int)t_type];
#ifdef _DEBUG
	if (currentPressed && previousPressed)
	{
		std::cout << getButtonName(t_type) << " Held" << std::endl;
		return ButtonState::Held;
	}
	else if (currentPressed && !previousPressed)
	{
		std::cout << getButtonName(t_type) << " Pressed" << std::endl;
		return ButtonState::Pressed;
	}
	else if (!currentPressed && previousPressed)
	{
		std::cout << getButtonName(t_type) << " Released" << std::endl;
		return ButtonState::Released;
	}
	else
	{
		return ButtonState::NotPressed;
	}
#else
	if (currentPressed && previousPressed) return ButtonState::Held;
	else if (currentPressed && !previousPressed) return ButtonState::Pressed;
	else if (!currentPressed && previousPressed) return ButtonState::Released;
	else return ButtonState::NotPressed;
#endif
}

std::string Controller::getButtonName(ButtonType t_type)
{
	std::string returnString = "Controller " + std::to_string(m_controllerIndex);
	switch (t_type)
	{
	case ButtonType::A:
		returnString += " A";
		break;
	case ButtonType::B:
		returnString += " B";
		break;
	case ButtonType::X:
		returnString += " X";
		break;
	case ButtonType::Y:
		returnString += " Y";
		break;
	case ButtonType::RB:
		returnString += " RB";
		break;
	case ButtonType::LB:
		returnString += " LB";
		break;
	case ButtonType::LeftThumbStickClick:
		returnString += " LS";
		break;
	case ButtonType::RightThumbStickClick:
		returnString += " RS";
		break;
	case ButtonType::DpadUp:
		returnString += " DPAD_UP";
		break;
	case ButtonType::DpadDown:
		returnString += " DPAD_DOWN";
		break;
	case ButtonType::DpadRight:
		returnString += " DPAD_RIGHT";
		break;
	case ButtonType::DpadLeft:
		returnString += " DPAD_LEFT";
		break;
	case ButtonType::Start:
		returnString += " START";
		break;
	case ButtonType::Back:
		returnString += " BACK";
		break;
	default:
		break;
	}
	return returnString;
}
