#include "stdafx.h"
#include "PlayerFourJoinCommand.h"
#include "..\include\PlayerFourJoinCommand.h"

void PlayerFourJoinCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent<Events::JoinGame>(Events::JoinGame{ 3 });
}
