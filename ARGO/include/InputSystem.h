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
	InputSystem(EventManager & t_em);
	~InputSystem();
	void update(Entity& t_entity, EventManager& t_em);
	void update(Entity& t_entity) {}
private:
	void handleInputs(InputComponent* t_inputComponent);
	void handleCommands(EventManager& t_em, InputComponent* t_input, Entity& t_entity);
 };

