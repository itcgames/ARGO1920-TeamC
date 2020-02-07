#pragma once
#include "BaseSystem.h"
#include "Entity.h"
#include "SDL.h"
#include "TransformComponent.h"
#include "InputComponent.h"
#include "ForceComponent.h"
#include <iostream>
#include "Debug.h"

class PhysicsSystem : public BaseSystem
{
public:
	~PhysicsSystem();
	void update(Entity& t_entity);

private:
	void checkBorder(TransformComponent* t_pos);
	void updateWithInput(ForceComponent* t_force, InputComponent* t_input);

	const float FRICTION_SCALAR = 0.95f;
};
