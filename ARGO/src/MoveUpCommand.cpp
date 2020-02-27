#include "stdafx.h"
#include "MoveUpCommand.h"
 
void MoveUpCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent(Events::PhysicsMove{ glm::vec2(0,-1), t_entity });
}
