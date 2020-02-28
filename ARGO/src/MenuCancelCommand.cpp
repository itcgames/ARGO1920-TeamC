#include "stdafx.h"
#include "MenuCancelCommand.h"
#include "..\include\MenuCancelCommand.h"

void MenuCancelCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent(MenuButtonPressed{ ButtonType::B });
}
