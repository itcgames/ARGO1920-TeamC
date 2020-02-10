#include "stdafx.h"
#include "InputHandler.h"
 

InputHandler::InputHandler()
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
	// returns empty map if anything else
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
void InputHandler::updateButtonCommand(ButtonType t_button, ButtonState t_state, Command* t_command)
{
	if (ButtonState::Pressed == t_state)
	{
		if (!m_buttonPressedCommands.empty())
		{
			m_buttonPressedCommands[t_button] = t_command;
		}
	}
	else if (ButtonState::Held == t_state)
	{
		if (!m_buttonHeldCommands.empty())
		{
			m_buttonHeldCommands[t_button] = t_command;
		}
	}
	else if (ButtonState::Released == t_state)
	{
		if (!m_buttonReleaseCommands.empty())
		{
			m_buttonReleaseCommands[t_button] = t_command;
		}
	}
}  
