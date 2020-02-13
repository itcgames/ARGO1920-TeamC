#pragma once
#include "InputComponent.h"
#include "SDL.h"
#include "Entity.h"
#include <iostream>
#include "Debug.h"
#include "BaseSystem.h"
 
class InputSystem : public BaseSystem
{
public:
	InputSystem();
	~InputSystem();
	void update(Entity& t_entity, EventManager& t_eventManager);
	void update(Entity& t_entity) {}
private:
	void handleInputs(InputComponent* t_inputComponent);
	void handleCommands(EventManager& t_eventManager, InputComponent* t_input, Entity& t_entity);
 };

