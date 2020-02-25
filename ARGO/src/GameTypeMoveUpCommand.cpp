#include "stdafx.h"
#include "GameTypeMoveUpCommand.h"

void GameTypeMoveUpCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent<GameTypeMoveButtons>(GameTypeMoveButtons{ MoveDirection::Up});
}
