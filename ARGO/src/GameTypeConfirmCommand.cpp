#include "stdafx.h"
#include "GameTypeConfirmCommand.h"

void GameTypeConfirmCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent<Events::GameTypeConfirm>(Events::GameTypeConfirm{});
}
