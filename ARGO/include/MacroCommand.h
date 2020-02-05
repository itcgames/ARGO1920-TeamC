#pragma once
#include "Command.h"
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

