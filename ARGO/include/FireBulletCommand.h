#pragma once
#include "Command.h"
#include "TransformComponent.h"
#include "InputComponent.h"
#include "Controller.h"
class FireBulletCommand :
	public Command
{
public:
	virtual void execute(Entity& t_entity, EventManager& t_eventManager);
};

