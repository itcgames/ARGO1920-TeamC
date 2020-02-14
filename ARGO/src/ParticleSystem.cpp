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
		partComp->setPosition(static_cast<TransformComponent*>(t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID)))

		for (int i = 0; i < partComp->getMaxParticles(); i++)
		{

			//Places More Particles if the Emitter is Emitting
			if (partComp->getEmitting() && !partComp->getParticleAlive(i))
			{
				partComp->setParticleMovement(i, randomDirectionVectorInRange(partComp->getAngle(), partComp->getAngleOffset()));
				if (m_placeParticleTimer > 1.0f)
				{
					partComp->setParticle(partComp->getEmitterPosition() + glm::vec2(partComp->randomOffset(), partComp->randomOffset()));
					m_placeParticleTimer = 0;
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
		m_placeParticleTimer += partComp->getParticlesPerSecond();
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

