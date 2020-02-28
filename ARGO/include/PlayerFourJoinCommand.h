#pragma once
#include "Command.h"
class PlayerFourJoinCommand :
	public Command
{
public:
	virtual void execute(Entity& t_entity, EventManager& t_eventManager);
};

