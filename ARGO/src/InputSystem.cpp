#include "stdafx.h"
#include "InputSystem.h"


InputSystem::~InputSystem()
{
	BaseSystem::~BaseSystem();
}

void InputSystem::update(Entity& t_entity)
{
#if (INPUT_SYS_DEBUG == 1)
	std::cout << "Calling InputComponent.update()" << std::endl;
#endif

	if (t_entity.getAllComps().at(COMPONENT_ID::INPUT_ID))
	{
		InputComponent* inputComp = static_cast<InputComponent*>(t_entity.getAllComps().at(COMPONENT_ID::INPUT_ID));
		inputComp->update();
	}
}
