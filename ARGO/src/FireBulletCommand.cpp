#include "stdafx.h"
#include "FireBulletCommand.h"

void FireBulletCommand::execute(Entity& t_entity, EventManager& t_eventManager)
{
	TransformComponent* transformComponent = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform));
	InputComponent* inputComp = static_cast<InputComponent*>(t_entity.getComponent(ComponentType::Input));
	Weapon weapon = static_cast<WeaponComponent*>(t_entity.getComponent(ComponentType::Weapon))->getCurrent();
	if (transformComponent)
	{
		Controller playerController = inputComp ? inputComp->getController() : Controller();
 		glm::vec2 direction = glm::vec2(std::cos(glm::radians(transformComponent->getRotation())), std::sin(glm::radians(transformComponent->getRotation())));
		t_eventManager.emitEvent(CreateBulletEvent{ t_entity, direction, 32, weapon, playerController });
	}
}
