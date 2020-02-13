#pragma once
#include "Command.h"
 
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
	// stack that holds list of current commands
	std::stack<Command*> m_commands;
	// stack to hold all executed commands, 
	// m_commands changes constantly as commands are executed and removed
	// m_commandHistory does not
	std::stack<Command*> m_commandHistory;
};

