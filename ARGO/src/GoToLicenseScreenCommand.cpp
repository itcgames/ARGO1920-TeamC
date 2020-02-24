#include "stdafx.h"
#include "GoToLicenseScreenCommand.h"

void GoToLicenseScreenCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	t_eventManager.emitEvent(ChangeScreen{ MenuStates::License });
}
