#pragma once
#include "Component.h"
#include "SDL.h"
#include "Controller.h"
#include "MacroCommand.h"

class InputComponent : public Component
{
public:
	InputComponent(std::map<ButtonType, Command*> t_buttonPressMap = std::map<ButtonType, Command*>(),
				   std::map<ButtonType, Command*> t_buttonHeldMap = std::map<ButtonType, Command*>(),
				   std::map<ButtonType, Command*> t_buttonReleasedMap = std::map<ButtonType, Command*>());
	~InputComponent();
	void update();
	Controller getController();
	std::stack<Command*> getCommands();
	void addCommand(Command* t_command);
	void popTopCommand();

	std::map<ButtonType, Command*> getButtonMap(ButtonState t_mapType);
	void setButtonMap(ButtonState t_mapType, std::map<ButtonType, Command*> t_map);
private:
	// Controller that handles input from connected controller
	Controller m_controller;
	// List of commands that are caused by inputs
	MacroCommand m_commands;
	// map that holds the commands for when buttons are pressed
	std::map<ButtonType, Command*> m_buttonPressedCommands;
	// map that holds the commands for when buttons are held
	std::map<ButtonType, Command*> m_buttonHeldCommands;
	// map that holds the commands for when buttons are released
	std::map<ButtonType, Command*> m_buttonReleaseCommands;
};

