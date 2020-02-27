#include "stdafx.h"
#include "MenuMoveRightCommand.h"
 
void MenuMoveRightCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent(Events::MenuMoveBetweenUI{ MoveDirection::Right });

}
