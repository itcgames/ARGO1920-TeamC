#include "stdafx.h"
#include "GoToMainMenuCommand.h"

void GoToMainMenuCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent(Events::ChangeScreen{ MenuStates::MainMenu });
}
