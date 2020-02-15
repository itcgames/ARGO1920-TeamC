#include "stdafx.h"
#include "InputComponent.h"


InputComponent::InputComponent(std::map<ButtonType, Command*> t_buttonPressMap,
							   std::map<ButtonType, Command*> t_buttonHeldMap,
							   std::map<ButtonType, Command*> t_buttonReleasedMap) :
	Component(ComponentType::Input),
	m_buttonPressedCommands{t_buttonPressMap},
	m_buttonHeldCommands{t_buttonHeldMap},
	m_buttonReleaseCommands{t_buttonReleasedMap},
	m_isActive{true}
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

bool InputComponent::getIsActive()
{
	return m_isActive;
}

void InputComponent::setIsActive(bool t_isActive)
{
	m_isActive = t_isActive;
}

