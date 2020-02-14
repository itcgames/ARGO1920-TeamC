#include "stdafx.h"
#include "CommandComponent.h"
#include "..\include\CommandComponent.h"

CommandComponent::CommandComponent() :
	Component(ComponentType::Command)
{
}

CommandComponent::~CommandComponent()
{
}

void CommandComponent::addCommand(Command* t_command)
{
	m_commands.add(t_command);
}

void CommandComponent::popTopCommand()
{
	m_commands.popTopCommand();
}

std::stack<Command*> CommandComponent::getCommands()
{
	return m_commands.getCommands();
}
