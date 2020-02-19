#include "stdafx.h"
#include "CommandSystem.h"

CommandSystem::CommandSystem()
{
}

CommandSystem::~CommandSystem()
{
	BaseSystem::~BaseSystem();
}

void CommandSystem::update(Entity& t_entity, EventManager& t_eventManager)
{
	CommandComponent* commandComp = static_cast<CommandComponent*>(t_entity.getComponent(ComponentType::Command));

	if (commandComp)
	{
		InputComponent* inputComp = static_cast<InputComponent*>(t_entity.getComponent(ComponentType::Input));
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
				if (inputComp)
				{
					t_eventManager.emitEvent(PhysicsMove{ glm::normalize(inputComp->getController().getCurrent().LeftThumbStick), t_entity });
				}
			}
			else if (typeid(*commandComp->getCommands().top()) == typeid(FireBulletCommand))
			{
				if (inputComp)
				{
					if (inputComp->getController().getCurrent().RightThumbStick != glm::vec2(0,0))
					{
						t_eventManager.emitEvent(createBulletEvent{ t_entity, glm::normalize(inputComp->getController().getCurrent().RightThumbStick), 32, 0 });
					}
				}
			}
			else if (typeid(*commandComp->getCommands().top()) == typeid(CloseWindowCommand))
			{
				t_eventManager.emitEvent(CloseWindow());
			}
			else continue;
			commandComp->popTopCommand();
		}
	}
}