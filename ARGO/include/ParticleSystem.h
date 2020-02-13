#pragma once
#include "BaseSystem.h"

#include "Debug.h"
#include "SDL.h"
#include "ParticleEmitterComponent.h"
#include "PrimitiveComponent.h"
#include "ColourComponent.h"
#include "Entity.h"
#include "CompEnums.h"
#include <random>


class ParticleSystem :
	public BaseSystem
{
public:
	~ParticleSystem();
	void update(Entity& t_entity);
	glm::vec2 randomDirectionVectorInRange(float t_angle, float t_angleOffset);
	float m_placeParticleTimer{ 0 };

};
