#pragma once
#include "Component.h"
#include "SDL.h"
#include "Controller.h"
#include "MacroCommand.h"

using ButtonCommandMap = std::map<ButtonType, Command*>;

class InputComponent : public Component
{
public:
	InputComponent(Controller& t_controller,
				   ButtonCommandMap t_buttonPressMap = ButtonCommandMap(),
				   ButtonCommandMap t_buttonHeldMap = ButtonCommandMap(),
				   ButtonCommandMap t_buttonReleasedMap = ButtonCommandMap());
	~InputComponent();
	void update();
	Controller& getController();
	ButtonCommandMap getButtonMap(ButtonState t_mapType);
	void setButtonMap(ButtonState t_mapType, ButtonCommandMap t_map);
private:
	// Controller that handles input from connected controller
	Controller& m_controller;
	// map that holds the commands for when buttons are pressed
	ButtonCommandMap m_buttonPressedCommands;
	// map that holds the commands for when buttons are held
	ButtonCommandMap m_buttonHeldCommands;
	// map that holds the commands for when buttons are released
	ButtonCommandMap m_buttonReleaseCommands;
};

