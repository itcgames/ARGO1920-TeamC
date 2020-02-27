#include "stdafx.h"
#include "MoveDownCommand.h"

void MoveDownCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent(Events::PhysicsMove{ glm::vec2(0,1), t_entity });
}
