#pragma once
#include "Command.h"
class MoveDownCommand :
	public Command
{
public:
	virtual void execute(Entity& t_entity, EventManager& t_eventManager);
};

