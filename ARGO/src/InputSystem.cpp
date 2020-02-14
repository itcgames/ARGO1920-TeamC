#include "stdafx.h"
#include "InputSystem.h" 

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
	BaseSystem::~BaseSystem();
}

void InputSystem::update(Entity& t_entity, EventManager& t_eventManager)
{

#if (INPUT_SYS_DEBUG == 1)
	std::cout << "Calling InputComponent.update()" << std::endl;
#endif

	if (t_entity.getAllComps().at(COMPONENT_ID::INPUT_ID))
	{
		InputComponent* inputComp = static_cast<InputComponent*>(t_entity.getAllComps().at(COMPONENT_ID::INPUT_ID));
		inputComp->update();
		handleInputs(inputComp);
		handleCommands(t_eventManager, inputComp, t_entity);
	}
}

void InputSystem::handleInputs(InputComponent* t_inputComponent)
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
				t_inputComponent->addCommand(buttonMap[(ButtonType)index]);
			}
		}
	}
	if (controller.getCurrent().LeftThumbStick != glm::vec2(0.0f, 0.0f))
	{
		t_inputComponent->addCommand(new AnalogMoveCommand());
	}
	if (controller.getCurrent().RightThumbStick != glm::vec2(0.0f, 0.0f))
	{
	}
}

void InputSystem::handleCommands(EventManager& t_eventManager, InputComponent* t_input, Entity& t_entity)
{
	while (!t_input->getCommands().empty())
	{
		if (typeid(*t_input->getCommands().top()) == typeid(MoveUpCommand))
		{
			t_eventManager.emitEvent(PhysicsMove{ glm::vec2(0,-1), t_entity });
		}
		else if (typeid(*t_input->getCommands().top()) == typeid(MoveDownCommand))
		{
			t_eventManager.emitEvent(PhysicsMove{ glm::vec2(0,1), t_entity });
		}
		else if (typeid(*t_input->getCommands().top()) == typeid(MoveLeftCommand))
		{
			t_eventManager.emitEvent(PhysicsMove{ glm::vec2(-1, 0), t_entity });
		}
		else if (typeid(*t_input->getCommands().top()) == typeid(MoveRightCommand))
		{
			t_eventManager.emitEvent(PhysicsMove{ glm::vec2(1, 0), t_entity });
		}
		else if (typeid(*t_input->getCommands().top()) == typeid(AnalogMoveCommand))
		{
			t_eventManager.emitEvent(PhysicsMove{ glm::normalize(t_input->getController().getCurrent().LeftThumbStick), t_entity });
		}
		else if (typeid(*t_input->getCommands().top()) == typeid(CloseWindowCommand))
		{
			t_eventManager.emitEvent(CloseWindow());
		}
		else if (typeid(*t_input->getCommands().top()) == typeid(FireBulletCommand))
		{
			std::cout << t_input->getController().getCurrent().RightThumbStick.x << "y: " << t_input->getController().getCurrent().RightThumbStick.y << std::endl;
			t_eventManager.emitEvent(createBulletEvent{ t_entity, glm::normalize(t_input->getController().getCurrent().RightThumbStick), Utilities::PROJECTILE_SPEED ,0 });
		}
		else continue;
		t_input->popTopCommand();
	}
}
