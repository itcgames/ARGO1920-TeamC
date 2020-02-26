#pragma once
#include "Command.h"
#include "InputComponent.h"
class ThrowGlowstickCommand :
	public Command
{
	virtual void execute(Entity& t_entity, EventManager& t_eventManager);
};

