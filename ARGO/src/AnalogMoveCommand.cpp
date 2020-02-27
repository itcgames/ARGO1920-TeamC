#include "stdafx.h"
#include "AnalogMoveCommand.h"

void AnalogMoveCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	InputComponent* inputComp = static_cast<InputComponent*>(t_entity.getComponent(ComponentType::Input));
	if (inputComp)
	{
		t_eventManager.emitEvent(Events::PhysicsMove{ glm::normalize(inputComp->getController().getCurrent().LeftThumbStick), t_entity });
	}
}
