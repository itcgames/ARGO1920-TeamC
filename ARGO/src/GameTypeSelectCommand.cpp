#include "stdafx.h"
#include "GameTypeSelectCommand.h"

void GameTypeSelectCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent<GameTypeSelectButton>(GameTypeSelectButton{});
}
