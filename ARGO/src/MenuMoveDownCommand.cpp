#include "stdafx.h"
#include "MenuMoveDownCommand.h"

void MenuMoveDownCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent(Events::MenuMoveBetweenUI{ MoveDirection::Down });
}
