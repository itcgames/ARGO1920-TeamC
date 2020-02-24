#pragma once
#include "Command.h"
class MenuMoveUpCommand :
	public Command
{
public:
	virtual void execute(Entity& t_entity, EventManager& t_eventManager);
};

