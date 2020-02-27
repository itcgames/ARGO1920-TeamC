#include "stdafx.h"
#include "GameTypeMoveRightCommand.h"

void GameTypeMoveRightCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent<Events::GameTypeMoveButtons>(Events::GameTypeMoveButtons{ MoveDirection::Right });

}
