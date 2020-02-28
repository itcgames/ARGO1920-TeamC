#include "stdafx.h"
#include "PlayerTwoJoinCommand.h"
#include "..\include\PlayerTwoJoinCommand.h"

void PlayerTwoJoinCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent<Events::JoinGame>(Events::JoinGame{ 1 });
}
