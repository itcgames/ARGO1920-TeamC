#pragma once
#include "Controller.h"
#include "MacroCommand.h"
/// <summary>
/// Class to handle inputes for the command pattern
/// </summary>
class InputHandler
{
public:
	InputHandler(std::map<ButtonType, Command*> t_buttonPressedCommands, std::map<ButtonType, Command*> t_buttonHeldCommands, std::map<ButtonType, Command*> t_buttonReleasedCommands);
	void setButtonMaps(std::map<ButtonType, Command*> t_buttonPressedCommands, std::map<ButtonType, Command*> t_buttonHeldCommands, std::map<ButtonType, Command*> t_buttonReleasedCommands);
	void setPressedButtonMap(std::map<ButtonType, Command*> t_newMap);
	void setReleasedButtonMap(std::map<ButtonType, Command*> t_newMap);
	void setHeldButtonMap(std::map<ButtonType, Command*> t_newMap);
	std::map<ButtonType, Command*> getButtonMap(ButtonState t_mapType);
	void handleControllerInput(Controller* t_controller);
private: 
	// maps that hold the corresponding commands based on buttontype
	// each map is for a different state
	std::map<ButtonType, Command*> m_buttonPressedCommands;
	std::map<ButtonType, Command*> m_buttonHeldCommands;
	std::map<ButtonType, Command*> m_buttonReleaseCommands;

	MacroCommand m_commands;
 };

