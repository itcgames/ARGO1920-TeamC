#include "stdafx.h"
#include "MacroCommand.h"

MacroCommand::MacroCommand()
{
}

void MacroCommand::add(Command* t_command)
{
	m_commands.push(t_command);
	m_commandHistory.push(t_command);
}

void MacroCommand::addAndExecute(Command* t_command)
{
	add(t_command);
	executeTop();
}

void MacroCommand::executeTop()
{
	m_commands.top()->execute();
}

std::stack<Command*> MacroCommand::getCommands()
{
	return m_commands;
}

void MacroCommand::popTopCommand()
{
	m_commands.pop();
}
