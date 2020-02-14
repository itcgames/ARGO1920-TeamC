#include "stdafx.h"
#include "ParticleSystem.h"
#include "ext.hpp"

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::update(Entity& t_entity)
{
	if (t_entity.getAllComps().at(COMPONENT_ID::PARTICLE_ID) && t_entity.getAllComps().at(COMPONENT_ID::PRIMITIVE_ID) && t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))
	{
		ParticleEmitterComponent* partComp = static_cast<ParticleEmitterComponent*>(t_entity.getAllComps().at(COMPONENT_ID::PARTICLE_ID));

		PrimitiveComponent* primtiveComp = static_cast<PrimitiveComponent*>(t_entity.getAllComps().at(COMPONENT_ID::PRIMITIVE_ID));
		partComp->setPosition(static_cast<TransformComponent*>(t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos());

		for (int i = 0; i < partComp->getMaxParticles(); i++)
		{

			//Places More Particles if the Emitter is Emitting
			if (partComp->getEmitting() && !partComp->getParticleAlive(partComp->getNextParticle()))
			{
				
				if (partComp->getPlaceParticleTimer() > 1.0f)
				{
					if(partComp->getRotating())
					{
						adjustParticleAngles((partComp->getAngle() + partComp->getRotating()), partComp);
					}
					partComp->setParticleMovement(partComp->getNextParticle(), randomDirectionVectorInRange(partComp->getAngle(), partComp->getAngleOffset()));
					partComp->setParticle(partComp->getEmitterPosition() + glm::vec2(randomOffset(partComp->getOffset()), partComp->getOffset()));
					partComp->setPlaceParticleTimer(partComp->getPlaceParticleTimer() - 1);
				}
			}

			if (partComp->getParticleAlive(i))
			{
				//Checks to see if the Particle should be killed
				if (partComp->isParticleDead(i))
				{
					partComp->killParticle(i);
				}
				//Updates the Particles
				partComp->updateParticle(i);
	
			}
		}
		partComp->setPlaceParticleTimer(partComp->getPlaceParticleTimer() + partComp->getParticlesPerSecond());
	}
}

glm::vec2 ParticleSystem::randomDirectionVectorInRange(float t_angle, float t_angleOffset)
{
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> distr(-t_angleOffset, t_angleOffset); // define the range
	int tempAngle = distr(eng);
	return  glm::vec2(glm::cos((t_angle + tempAngle) * M_PI / 180), glm::sin((t_angle + tempAngle) * M_PI / 180));

}

int ParticleSystem::randomOffset(int t_offsetAmount)
{
	return (rand() % (t_offsetAmount * 2) + 1) - t_offsetAmount;
}

void ParticleSystem::adjustParticleAngles(float t_newAngle, ParticleEmitterComponent* t_partComp)
{
	t_partComp->setAngle(t_newAngle);
}

