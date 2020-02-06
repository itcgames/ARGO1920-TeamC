#pragma once
#include "BaseSystem.h"
#include "Entity.h"
#include "SDL.h"
#include "TransformComponent.h"
#include "InputComponent.h"
#include <iostream>
#include "Debug.h"

class PhysicsSystem : public BaseSystem
{
public:
	~PhysicsSystem();
	void update(Entity& t_entity);

private:
	void checkBorder(TransformComponent* t_pos);
	void updateWithInput(TransformComponent* t_pos, InputComponent* t_input);
};
