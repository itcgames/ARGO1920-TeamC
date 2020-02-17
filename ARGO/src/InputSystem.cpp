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

#if (INPUT_SYS_DEBUG == 1)
	std::cout << "Calling InputComponent.update()" << std::endl;
#endif

	if (t_entity.getAllComps().at(COMPONENT_ID::INPUT_ID) && t_entity.getAllComps().at(COMPONENT_ID::COMMAND_ID))
	{
		InputComponent* inputComp = static_cast<InputComponent*>(t_entity.getAllComps().at(COMPONENT_ID::INPUT_ID));
		CommandComponent* commandComp = static_cast<CommandComponent*>(t_entity.getAllComps().at(COMPONENT_ID::COMMAND_ID));
		inputComp->update();
		handleInputs(inputComp, commandComp);
	}
}

void InputSystem::handleInputs(InputComponent* t_inputComponent, CommandComponent* t_commandComponent)
{
	Controller controller = t_inputComponent->getController();
	for (int index = 0; index < Utilities::NUMBER_OF_CONTROLLER_BUTTONS; index++)
	{
		ButtonState stateOfButton = controller.getButtonState((ButtonType)index);
		std::map<ButtonType, Command*> buttonMap = t_inputComponent->getButtonMap(stateOfButton);
		if (buttonMap != std::map<ButtonType, Command*>())
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
