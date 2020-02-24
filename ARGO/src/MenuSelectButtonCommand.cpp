#include "stdafx.h"
#include "MenuSelectButtonCommand.h"

void MenuSelectButtonCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent(MenuSelectButton{ t_entity });
}
