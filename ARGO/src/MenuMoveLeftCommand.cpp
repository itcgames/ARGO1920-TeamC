#include "stdafx.h"
#include "MenuMoveLeftCommand.h"
#include "..\include\MenuMoveLeftCommand.h"

void MenuMoveLeftCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent(MenuMoveBetweenUI{ MoveDirection::Left });
}
