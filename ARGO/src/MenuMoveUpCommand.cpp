#include "stdafx.h"
#include "MenuMoveUpCommand.h"

void MenuMoveUpCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent(MenuMoveBetweenUI{ MoveDirection::Up });
}
