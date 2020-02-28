#include "stdafx.h"
#include "GoToGameScreenCommand.h"
#include "..\include\GoToGameScreenCommand.h"

void GoToGameScreenCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent(ChangeScreen{ MenuStates::Game });
}
