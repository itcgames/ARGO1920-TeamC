#include "stdafx.h"
#include "CommandSystem.h"
#include "TransformComponent.h"
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
 		while (!commandComp->getCommands().empty())
		{
			commandComp->executeTopCommand(t_entity, t_eventManager);
 			commandComp->popTopCommand();
		}
	}
}