#include "stdafx.h"
#include "InputComponent.h"


InputComponent::InputComponent(Controller& t_controller, 
							   ButtonCommandMap t_buttonPressMap,
							   ButtonCommandMap t_buttonHeldMap,
							   ButtonCommandMap t_buttonReleasedMap) :
	Component(ComponentType::Input),
	m_controller{t_controller},
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

Controller& InputComponent::getController()
{
	return m_controller;
} 

ButtonCommandMap InputComponent::getButtonMap(ButtonState t_mapType)
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
	return ButtonCommandMap();
}

void InputComponent::setButtonMap(ButtonState t_mapType, ButtonCommandMap t_map)
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
