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

void MacroCommand::addAndExecute(Command* t_command, Entity& t_entity, EventManager& t_eventManager)
{
	add(t_command);
	executeTop(t_entity, t_eventManager);
}

void MacroCommand::executeTop(Entity& t_entity, EventManager& t_eventManager)
{
	m_commands.top()->execute(t_entity, t_eventManager);
}

void MacroCommand::executeTopAndPop(Entity& t_entity, EventManager& t_eventManager)
{
	executeTop(t_entity, t_eventManager);
	popTopCommand();
}

std::stack<Command*> MacroCommand::getCommands()
{
	return m_commands;
}

void MacroCommand::popTopCommand()
{
	m_commands.pop();
}
