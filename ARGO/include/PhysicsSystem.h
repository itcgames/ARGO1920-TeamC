#pragma once
#include "BaseSystem.h"
#include "Entity.h"
#include "SDL.h"
#include "TransformComponent.h"
#include "InputComponent.h"
#include "ForceComponent.h"
#include <iostream>
#include "Debug.h"
#include "Utilities.h"

class PhysicsSystem : public BaseSystem
{
public:
	PhysicsSystem(EventManager& t_eventManager);
	~PhysicsSystem();
	void update(Entity& t_entity, float t_dt);
	void update(Entity& t_entity);

private: 

	void updateWithInput(const PhysicsMove& t_event);
 	const float FRICTION_SCALAR = 0.9f;
};
