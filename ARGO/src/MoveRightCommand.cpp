#include "stdafx.h"
#include "MoveRightCommand.h"

void MoveRightCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent(PhysicsMove{ glm::vec2(1, 0), t_entity });
}
