#pragma once
#include "Command.h"
#include "InputComponent.h"
class FireBulletCommand :
	public Command
{
public:
	virtual void execute(Entity& t_entity, EventManager& t_eventManager);
};

