#include "stdafx.h"
#include "MoveLeftCommand.h"

void MoveLeftCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent(Events::PhysicsMove{ glm::vec2(-1, 0), t_entity });
}
