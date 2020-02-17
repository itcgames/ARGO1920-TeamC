#include "stdafx.h"
#include "CommandSystem.h"
#include "..\include\CommandSystem.h"

CommandSystem::CommandSystem()
{
}

CommandSystem::~CommandSystem()
{
	BaseSystem::~BaseSystem();
}

void CommandSystem::update(Entity& t_entity, EventManager& t_eventManager)
{
	if (t_entity.getAllComps().at(COMPONENT_ID::COMMAND_ID))
	{
		CommandComponent* commandComp = static_cast<CommandComponent*>(t_entity.getAllComps().at(COMPONENT_ID::COMMAND_ID));
		while (!commandComp->getCommands().empty())
		{
			if (typeid(*commandComp->getCommands().top()) == typeid(MoveUpCommand))
			{
				t_eventManager.emitEvent(PhysicsMove{ glm::vec2(0,-1), t_entity });
			}
			else if (typeid(*commandComp->getCommands().top()) == typeid(MoveDownCommand))
			{
				t_eventManager.emitEvent(PhysicsMove{ glm::vec2(0,1), t_entity });
			}
			else if (typeid(*commandComp->getCommands().top()) == typeid(MoveLeftCommand))
			{
				t_eventManager.emitEvent(PhysicsMove{ glm::vec2(-1, 0), t_entity });
			}
			else if (typeid(*commandComp->getCommands().top()) == typeid(MoveRightCommand))
			{
				t_eventManager.emitEvent(PhysicsMove{ glm::vec2(1, 0), t_entity });
			}
			else if (typeid(*commandComp->getCommands().top()) == typeid(AnalogMoveCommand))
			{
				if (t_entity.getAllComps().at(COMPONENT_ID::INPUT_ID))
				{
					InputComponent* inputComp = static_cast<InputComponent*>(t_entity.getAllComps().at(COMPONENT_ID::INPUT_ID));
 					t_eventManager.emitEvent(PhysicsMove{ glm::normalize(inputComp->getController().getCurrent().LeftThumbStick), t_entity });
				}
			}
			else if (typeid(*commandComp->getCommands().top()) == typeid(CloseWindowCommand))
			{
				t_eventManager.emitEvent(CloseWindow());
			}
			else if (typeid(*commandComp->getCommands().top()) == typeid(MenuMoveUpCommand))
			{
				t_eventManager.emitEvent(MenuMoveButtonsUpDown{ t_entity, false });
			}
			else if (typeid(*commandComp->getCommands().top()) == typeid(MenuMoveDownCommand))
			{
				t_eventManager.emitEvent(MenuMoveButtonsUpDown{ t_entity, true });
			}
			else if (typeid(*commandComp->getCommands().top()) == typeid(MenuSelectButtonCommand))
			{
				t_eventManager.emitEvent(MenuSelectButton{ t_entity });
			}
			else continue;
			commandComp->popTopCommand();
		}
	}
}