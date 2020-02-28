#include "stdafx.h"
#include "AnalogMoveCommand.h"

void AnalogMoveCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	InputComponent* inputComp = static_cast<InputComponent*>(t_entity.getComponent(ComponentType::Input));
	if (inputComp)
	{
		FSMComponent* fsm = static_cast<FSMComponent*>(t_entity.getComponent(ComponentType::FSM));
		if (fsm)
		{
			fsm->getFSM().setMoved(true);
		}
		t_eventManager.emitEvent(PhysicsMove{ glm::normalize(inputComp->getController().getCurrent().LeftThumbStick), t_entity });
	}
}
