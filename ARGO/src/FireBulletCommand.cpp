#include "stdafx.h"
#include "FireBulletCommand.h"

void FireBulletCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	InputComponent* inputComp = static_cast<InputComponent*>(t_entity.getComponent(ComponentType::Input));
	if (inputComp)
	{
		if (inputComp->getController().getCurrent().RightThumbStick != glm::vec2(0, 0))
		{
			t_eventManager.emitEvent(CreateBulletEvent{ t_entity, glm::normalize(inputComp->getController().getCurrent().RightThumbStick), 32, 0, inputComp->getController() });
		}
	}
}
