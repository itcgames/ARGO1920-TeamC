#include "stdafx.h"
#include "InputHandler.h"
 
/// <summary>
/// Constructor for Input Handler
/// Pass in a map that contains the controller button and corresponding command
/// Do this for the three different controller options
/// </summary>
/// <param name="t_buttonPressedCommands">commands for when button is pressed</param>
/// <param name="t_buttonHeldCommands">commands for when button is held</param>
/// <param name="t_buttonReleasedCommands">commands for when button is released</param>
InputHandler::InputHandler(/*std::map<ButtonType, Command*> t_buttonPressedCommands,
						   std::map<ButtonType, Command*> t_buttonHeldCommands,
						   std::map<ButtonType, Command*> t_buttonReleasedCommands*/)
	/*:
	m_buttonPressedCommands{t_buttonPressedCommands},
	m_buttonHeldCommands{t_buttonHeldCommands},
	m_buttonReleaseCommands{t_buttonReleasedCommands}*/
{
}

void InputHandler::setButtonMaps(std::map<ButtonType, Command*> t_buttonPressedCommands, std::map<ButtonType, Command*> t_buttonHeldCommands, std::map<ButtonType, Command*> t_buttonReleasedCommands)
{
	setPressedButtonMap(t_buttonPressedCommands);
	setReleasedButtonMap(t_buttonReleasedCommands);
	setHeldButtonMap(t_buttonHeldCommands);
}

void InputHandler::setPressedButtonMap(std::map<ButtonType, Command*> t_newMap)
{
	m_buttonPressedCommands = t_newMap;
}

void InputHandler::setReleasedButtonMap(std::map<ButtonType, Command*> t_newMap)
{
	m_buttonReleaseCommands = t_newMap;
}

void InputHandler::setHeldButtonMap(std::map<ButtonType, Command*> t_newMap)
{
	m_buttonHeldCommands = t_newMap;
}

std::map<ButtonType, Command*> InputHandler::getButtonMap(ButtonState t_mapType)
{
	if (ButtonState::Pressed == t_mapType)
	{
		return m_buttonPressedCommands;
	}
	else if (ButtonState::Released == t_mapType)
	{
		return m_buttonReleaseCommands;
	}
	else if (ButtonState::Held == t_mapType)
	{
		return m_buttonHeldCommands;
	}
	return std::map<ButtonType, Command*>();
}

void InputHandler::handleControllerInput(Controller* t_controller)
{
	for (int index = 0; index < (int)ButtonType::Count; index++)
	{
		// Get the state of each button
		ButtonState stateOfButton = t_controller->getButtonState((ButtonType)index);
		if (ButtonState::Pressed == stateOfButton && !m_buttonPressedCommands.empty())
		{
			if (m_buttonPressedCommands[(ButtonType)index] != nullptr)
			{
				// Execute Command of appropiate button when pressed
				m_commands.addAndExecute(m_buttonPressedCommands[(ButtonType)index]);
			}
		}
		else if (ButtonState::Held == stateOfButton && !m_buttonHeldCommands.empty())
		{
			if (m_buttonHeldCommands[(ButtonType)index] != nullptr)
			{
				// Execute Command of appropiate button when pressed
				m_commands.addAndExecute(m_buttonHeldCommands[(ButtonType)index]);
			}
		}
		else if (ButtonState::Released == stateOfButton && !m_buttonReleaseCommands.empty())
		{
			if (m_buttonReleaseCommands[(ButtonType)index] != nullptr)
			{
				// Execute Command of appropiate button when pressed
				m_commands.addAndExecute(m_buttonReleaseCommands[(ButtonType)index]);
			}
		}
	}
} 