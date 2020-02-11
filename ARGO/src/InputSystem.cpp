#include "stdafx.h"
#include "InputSystem.h" 

InputSystem::InputSystem(EventManager& t_em)
{
	//t_em.subscribe<TEST>(
	//	std::bind(&InputSystem::test2, this, std::placeholders::_1)
	//	);
}

InputSystem::~InputSystem()
{
	BaseSystem::~BaseSystem();
}

void InputSystem::update(Entity& t_entity, EventManager& t_em)
{

#if (INPUT_SYS_DEBUG == 1)
	std::cout << "Calling InputComponent.update()" << std::endl;
#endif

	if (t_entity.getAllComps().at(COMPONENT_ID::INPUT_ID))
	{
		InputComponent* inputComp = static_cast<InputComponent*>(t_entity.getAllComps().at(COMPONENT_ID::INPUT_ID));
		inputComp->update();
		handleInputs(inputComp);
		handleCommands(t_em, inputComp, t_entity);
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
		t_inputComponent->addCommand(new AnalogMoveCommand());
		// add movement componet;
	}
	if (controller.getCurrent().RightThumbStick != glm::vec2(0.0f, 0.0f))
	{
		// add movement component;
	}
}

void InputSystem::handleCommands(EventManager& t_eventManager, InputComponent* t_input, Entity& t_entity)
{
	
	while (!t_input->getCommands().empty())
	{
 		if (!t_input->getCommands().empty() && typeid(*t_input->getCommands().top()) == typeid(MoveUpCommand))
		{
			t_eventManager.emit(PhysicsMove{ glm::vec2(0,-1), t_entity });
 			t_input->popTopCommand();
		}
		if (!t_input->getCommands().empty() && typeid(*t_input->getCommands().top()) == typeid(MoveDownCommand))
		{
			t_eventManager.emit(PhysicsMove{ glm::vec2(0,1), t_entity });
 			t_input->popTopCommand();
		}
		if (!t_input->getCommands().empty() && typeid(*t_input->getCommands().top()) == typeid(MoveLeftCommand))
		{
			t_eventManager.emit(PhysicsMove{ glm::vec2(-1, 0), t_entity });
 			t_input->popTopCommand();
		}
		if (!t_input->getCommands().empty() && typeid(*t_input->getCommands().top()) == typeid(MoveRightCommand))
		{
			t_eventManager.emit(PhysicsMove{ glm::vec2(1, 0), t_entity });
 			t_input->popTopCommand();
		}

		if (!t_input->getCommands().empty() && typeid(*t_input->getCommands().top()) == typeid(AnalogMoveCommand))
		{
			t_eventManager.emit(PhysicsMove{ t_input->getController().getCurrent().LeftThumbStick, t_entity });
			t_input->popTopCommand();
		}
	}
}
