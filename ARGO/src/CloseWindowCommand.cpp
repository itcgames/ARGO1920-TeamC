#include "stdafx.h"
#include "CloseWindowCommand.h"

void CloseWindowCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent(CloseWindow());
}
