#pragma once
#include <iostream>
#include "SDL.h"
#include "Debug.h"
#include "BaseSystem.h"
#include "Entity.h"
#include "TransformComponent.h"

class AiSystem : public BaseSystem
{
public:
	~AiSystem();
	void update(Entity& t_e);
private:

};

