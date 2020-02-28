#include "stdafx.h"
#include "PlayerOneJoinCommand.h"
#include "..\include\PlayerOneJoinCommand.h"

void PlayerOneJoinCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent<Events::JoinGame>(Events::JoinGame{ 0 });
}
