#include "stdafx.h"
#include "GameTypeMoveDownCommand.h"

void GameTypeMoveDownCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent<Events::GameTypeMoveButtons>(Events::GameTypeMoveButtons{ MoveDirection::Down });
}
