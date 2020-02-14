#pragma once
#include "InputComponent.h"
#include "CommandComponent.h"
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
	void update(Entity& t_entity);
private:
	void handleInputs(InputComponent* t_inputComponent, CommandComponent* t_commandComponent);
 };

