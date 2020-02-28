#pragma once
#include "Entity.h"
#include "TransformComponent.h"
#include "CommandComponent.h"
#include "ColourComponent.h"
#include "InputComponent.h"
#include "AiComponent.h"
#include "HealthComponent.h"
#include "ForceComponent.h"
#include "ColliderCircleComponent.h"
#include "ParticleEmitterComponent.h"
#include "FactoryStatSheet.h"
#include "TagComponent.h"
#include "PrimitiveComponent.h"
#include "VisualComponent.h"
#include "PickUpComponent.h"
#include "Controller.h"
#include "FireRateComponent.h"
#include "WeaponComponent.h"
#include "FSMComponent.h"
#include <SDL_image.h>

class EntityFactory
{
public:
	virtual void createPlayer(Entity& t_entity) = 0;
	virtual void createEnemy(int t_type, Entity& t_entity) = 0;
	virtual void createDrop(int t_type, Entity& t_entity) = 0;

};

