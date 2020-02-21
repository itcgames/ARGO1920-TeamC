#include "stdafx.h"
#include "InputSystem.h" 

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
	BaseSystem::~BaseSystem();
}

void InputSystem::update(Entity& t_entity)
{

#ifdef INPUT_SYS_DEBUG
	//std::cout << "Calling InputComponent.update()" << std::endl;
#endif

	InputComponent* inputComp = static_cast<InputComponent*>(t_entity.getComponent(ComponentType::Input));
	CommandComponent* commandComp = static_cast<CommandComponent*>(t_entity.getComponent(ComponentType::Command));

	if (inputComp != nullptr && commandComp != nullptr)
	{
		inputComp->update();
		handleInputs(inputComp, commandComp);
	}
}

void InputSystem::handleInputs(InputComponent* t_inputComponent, CommandComponent* t_commandComponent)
{
	Controller& controller = t_inputComponent->getController();
	for (int index = 0; index < Utilities::NUMBER_OF_CONTROLLER_BUTTONS; index++)
	{
		ButtonState stateOfButton = controller.getButtonState((ButtonType)index);
		ButtonCommandMap buttonMap = t_inputComponent->getButtonMap(stateOfButton);
		if (!buttonMap.empty())
		{
			if (buttonMap[(ButtonType)index] != nullptr)
			{
				t_commandComponent->addCommand(buttonMap[(ButtonType)index]);
			}
		}
	}
	if (controller.getCurrent().LeftThumbStick != glm::vec2(0.0f, 0.0f))
	{
		t_commandComponent->addCommand(new AnalogMoveCommand());
	}
	if (controller.getCurrent().RightThumbStick != glm::vec2(0.0f, 0.0f))
	{
	}
}
