#pragma once
#include "Controller.h"
#include "MacroCommand.h"
/// <summary>
/// Class to handle inputes for the command pattern
/// </summary>
class InputHandler
{
public:
	InputHandler();
	void setButtonMaps(std::map<ButtonType, Command*> t_buttonPressedCommands,
					   std::map<ButtonType, Command*> t_buttonHeldCommands,
					   std::map<ButtonType, Command*> t_buttonReleasedCommands);
	void setPressedButtonMap(std::map<ButtonType, Command*> t_newMap);
	void setReleasedButtonMap(std::map<ButtonType, Command*> t_newMap);
	void setHeldButtonMap(std::map<ButtonType, Command*> t_newMap);
	void setAxisMovedMap(std::map<AxisType, Command*> t_newMap);
	std::map<ButtonType, Command*> getButtonMap(ButtonState t_mapType);
	void updateButtonCommand(ButtonType t_button, ButtonState t_state, Command* t_command);
	void updateAxisCommand(AxisType t_axis, Command* t_command);

	void handleControllerInput(Controller* t_controller);

private: 
	// maps that hold the corresponding commands based on buttontype
	// each map is for a different state
	std::map<ButtonType, Command*> m_buttonPressedCommands;
	std::map<ButtonType, Command*> m_buttonHeldCommands;
	std::map<ButtonType, Command*> m_buttonReleaseCommands;
	std::map<AxisType, Command*> m_axisMovedCommands;

	MacroCommand m_commands;
 };
