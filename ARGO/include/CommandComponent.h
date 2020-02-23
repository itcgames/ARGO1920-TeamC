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
	void executeTopCommand(Entity& t_entity, EventManager& t_eventManager);
	std::stack<Command*> getCommands();

private:
	MacroCommand m_commands;
};

