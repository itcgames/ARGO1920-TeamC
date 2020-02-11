#pragma once
#include "Command.h"
#include "MoveDownCommand.h"
#include "MoveUpCommand.h"
#include "MoveLeftCommand.h"
#include "MoveRightCommand.h"
#include "AnalogMoveCommand.h"

class MacroCommand
{
public:
	MacroCommand();
	void add(Command* t_command);
	void addAndExecute(Command* t_command);
	void executeTop();
	std::stack<Command*> getCommands();
	void popTopCommand();
private:
	std::stack<Command*> m_commands;
};

