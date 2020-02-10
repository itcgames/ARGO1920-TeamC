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
		handleInputs(inputComp);
	}
}

void InputSystem::handleInputs(InputComponent* t_inputComponent)
{
	auto controller = t_inputComponent->getController();
	for (int index = 0; index < (int)ButtonType::Count; index++)
	{
		ButtonState stateOfButton = controller.getButtonState((ButtonType)index);
		std::map<ButtonType,Command*> buttonMap = t_inputComponent->getButtonMap(stateOfButton);
		if (buttonMap != std::map<ButtonType,Command*>())
		{
			if (buttonMap[(ButtonType)index] != nullptr)
			{
				t_inputComponent->addCommand(buttonMap[(ButtonType)index]);
			}
		} 
	}
	if (controller.getCurrent().LeftThumbStick != glm::vec2(0.0f, 0.0f))
	{
		// add movement componet;
	}
	if (controller.getCurrent().RightThumbStick != glm::vec2(0.0f, 0.0f))
	{
		// add movement component;
	}
}
