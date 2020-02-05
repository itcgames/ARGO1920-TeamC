#include "stdafx.h"
#include "InputSystem.h"


InputSystem::~InputSystem()
{
	BaseSystem::~BaseSystem();
}

void InputSystem::update(Entity& t_e)
{
#if (INPUT_SYS_DEBUG == 1)
	std::cout << "Updating input for entity with id: " << m_entities[i]->getId() << std::endl;
#endif
	dynamic_cast<InputComponent*>(t_e.getComponent(ComponentType::Input))->update();
}
