#include "stdafx.h"
#include "GameTypeMoveLeftCommand.h"
#include "..\include\GameTypeMoveLeftCommand.h"

void GameTypeMoveLeftCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent<GameTypeMoveButtons>(GameTypeMoveButtons{ MoveDirection::Left });
}
