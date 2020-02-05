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
	~InputSystem();
	void update(Entity& t_e);
private:

};

