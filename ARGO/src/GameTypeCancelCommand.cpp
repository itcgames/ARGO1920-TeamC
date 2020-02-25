#include "stdafx.h"
#include "GameTypeCancelCommand.h"
#include "..\include\GameTypeCancelCommand.h"

void GameTypeCancelCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent<GameTypeCancel>(GameTypeCancel{});
}
