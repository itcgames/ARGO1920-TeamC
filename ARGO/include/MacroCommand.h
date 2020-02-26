#pragma once
#include "Command.h"
#include "AnalogMoveCommand.h"
#include "AnalogRotateCommand.h"
#include "CloseWindowCommand.h"
#include "FireBulletCommand.h"
#include "GoToLicenseScreenCommand.h"
#include "GoToMainMenuCommand.h"
#include "MenuMoveDownCommand.h"
#include "MenuMoveUpCommand.h"
#include "MenuSelectButtonCommand.h"
#include "MoveDownCommand.h"
#include "MoveLeftCommand.h"
#include "MoveRightCommand.h"
#include "MoveUpCommand.h"
#include "ThrowGlowstickCommand.h"
 
class MacroCommand
{
public:
	MacroCommand();
	void add(Command* t_command);
	void addAndExecute(Command* t_command, Entity& t_entity, EventManager& t_eventManager);
	void executeTop(Entity& t_entity, EventManager& t_eventManager);
	void executeTopAndPop(Entity& t_entity, EventManager& t_eventManager);
	void popTopCommand();
	std::stack<Command*> getCommands();
private:
	// stack that holds list of current commands
	std::stack<Command*> m_commands;
	// stack to hold all executed commands, 
	// m_commands changes constantly as commands are executed and removed
	// m_commandHistory does not
	std::stack<Command*> m_commandHistory;
};

