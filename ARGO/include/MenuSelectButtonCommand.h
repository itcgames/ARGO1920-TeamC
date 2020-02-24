#pragma once
#include "Command.h"
class MenuSelectButtonCommand :
	public Command
{
public:
	virtual void execute(Entity& t_entity, EventManager& t_eventManager);
};

