#pragma once
#include "Command.h"
#include "APressedCommand.h"
#include "BPressedCommand.h"
#include "XPressedCommand.h"
#include "YPressedCommand.h"
class MacroCommand
{
public:
	MacroCommand();
	void add(Command* t_command);
	void addAndExecute(Command* t_command);
	void executeTop();
private:
	std::stack<Command*> m_commands;
};

