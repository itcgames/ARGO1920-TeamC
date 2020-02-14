#pragma once
#include "Component.h"
#include "MacroCommand.h"
class CommandComponent :
	public Component
{
public:
	CommandComponent();
	~CommandComponent();

	void addCommand(Command* t_command);
	void popTopCommand();
	std::stack<Command*> getCommands();

private:
	MacroCommand m_commands;
};

