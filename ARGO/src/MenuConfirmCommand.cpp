#include "stdafx.h"
#include "MenuConfirmCommand.h"
#include "..\include\MenuConfirmCommand.h"

void MenuConfirmCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent(MenuButtonPressed{ ButtonType::Start });
}
