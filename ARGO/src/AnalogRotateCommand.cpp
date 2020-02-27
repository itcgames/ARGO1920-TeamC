#include "stdafx.h"
#include "AnalogRotateCommand.h"

void AnalogRotateCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	InputComponent* inputComp = static_cast<InputComponent*>(t_entity.getComponent(ComponentType::Input));
	if (inputComp)
	{
		t_eventManager.emitEvent(Events::PhysicsRotate{ glm::normalize(inputComp->getController().getCurrent().RightThumbStick), t_entity });
	}
}
