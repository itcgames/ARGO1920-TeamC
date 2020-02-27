#pragma once
#include "BaseSystem.h"

#include "Debug.h"
#include "SDL.h"
#include "ParticleEmitterComponent.h"
#include "PrimitiveComponent.h"
#include "ColourComponent.h"
#include "TransformComponent.h"
#include "Entity.h"
#include "CompEnums.h"
#include <random>


class ParticleSystem :
	public BaseSystem
{
public:
	~ParticleSystem();
	void update(Entity& t_entity, float t_dt);
	void update(Entity& t_entity);
	glm::vec2 randomDirectionVectorInRange(float t_angle, float t_angleOffset, int t_offsetSpeed);
	glm::uvec4 randomColourBetweenTwoColours(ColourComponent* t_colourComp);
	int randomOffset(int t_offsetAmount);
	void adjustParticleAngles(float t_newAngle, ParticleEmitterComponent* t_partComp);
	float randomFloatBetweenTwoValue(PrimitiveComponent* t_primComp);

};
