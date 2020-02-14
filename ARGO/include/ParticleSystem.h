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
	void update(Entity& t_entity);
	glm::vec2 randomDirectionVectorInRange(float t_angle, float t_angleOffset);//Generates a random unit vector within the range of angles. t_angle - t_angleoffset to  t_angle + t_angleoffset
	int randomOffset(int t_offsetAmount);//Returns a random value set by the offsetAmount Value 
	void adjustParticleAngles(float t_newAngle, ParticleEmitterComponent* t_partComp);//Pass in an angle and a reference to a Particle Component to change the angle to the given angle.

};
