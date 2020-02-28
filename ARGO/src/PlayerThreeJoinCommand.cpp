#include "stdafx.h"
#include "PlayerThreeJoinCommand.h"
#include "..\include\PlayerThreeJoinCommand.h"

void PlayerThreeJoinCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent<Events::JoinGame>(Events::JoinGame{ 2 });
}
