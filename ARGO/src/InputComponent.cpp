#include "stdafx.h"
#include "InputComponent.h"


InputComponent::InputComponent(std::map<ButtonType, Command*> t_buttonPressMap,
							   std::map<ButtonType, Command*> t_buttonHeldMap,
							   std::map<ButtonType, Command*> t_buttonReleasedMap) :
	Component(ComponentType::Input),
	m_buttonPressedCommands{t_buttonPressMap},
	m_buttonHeldCommands{t_buttonHeldMap},
	m_buttonReleaseCommands{t_buttonReleasedMap}
{
}

InputComponent::~InputComponent()
{
}

void InputComponent::update()
{ 
	m_controller.update();
} 

Controller InputComponent::getController()
{
	return m_controller;
} 

std::stack<Command*> InputComponent::getCommands()
{
	return m_commands.getCommands();
}

void InputComponent::addCommand(Command* t_command)
{
	m_commands.add(t_command);
}

void InputComponent::popTopCommand()
{
	m_commands.popTopCommand();
}

std::map<ButtonType, Command*> InputComponent::getButtonMap(ButtonState t_mapType)
{
	if (ButtonState::Pressed == t_mapType)
	{
		return m_buttonPressedCommands;
	}
	else if (ButtonState::Held == t_mapType)
	{
		return m_buttonHeldCommands;
	}
	else if (ButtonState::Released == t_mapType)
	{
		return m_buttonReleaseCommands;
	}
	// returns empty map if anything else
	return std::map<ButtonType, Command*>();
}

void InputComponent::setButtonMap(ButtonState t_mapType, std::map<ButtonType, Command*> t_map)
{
	if (ButtonState::Pressed == t_mapType)
	{
		m_buttonPressedCommands = t_map;
	}
	else if (ButtonState::Held == t_mapType)
	{
		m_buttonHeldCommands = t_map;
	}
	else if (ButtonState::Released == t_mapType)
	{
		m_buttonReleaseCommands = t_map;
	}
}

