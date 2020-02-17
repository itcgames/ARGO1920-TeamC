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
		ParticleEmitterComponent* particleComp = static_cast<ParticleEmitterComponent*>(t_entity.getAllComps().at(COMPONENT_ID::PARTICLE_ID));
		PrimitiveComponent* primtiveComp = static_cast<PrimitiveComponent*>(t_entity.getAllComps().at(COMPONENT_ID::PRIMITIVE_ID));
		particleComp->setPosition(static_cast<TransformComponent*>(t_entity.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos());//Sets the Emitter to the entity's tranform position. This makes it move with mobile entities.
		for (int i = 0; i < particleComp->getMaxParticles(); i++)
		{
			//If the Emitter is emitting and the currently queded particle is not already alive
			if (particleComp->getEmitting() && !particleComp->getParticleAlive(particleComp->getNextParticle()))
			{
				//And it is time to place a new particle
				if (particleComp->getPlaceParticleTimer() > 1.0f)
				{
					if (particleComp->getRotating())//Changes the angle if the Emitter is rotating
					{
						adjustParticleAngles((particleComp->getAngle() + particleComp->getRotating()), particleComp);
					}
					//Set's the particles movement, places it and decrements the placeTimer
					particleComp->setParticleMovement(particleComp->getNextParticle(), randomDirectionVectorInRange(particleComp->getAngle(), particleComp->getAngleOffset()));
					particleComp->setParticle(particleComp->getEmitterPosition() + glm::vec2(randomOffset(particleComp->getOffset()), particleComp->getOffset()));
					particleComp->setPlaceParticleTimer(particleComp->getPlaceParticleTimer() - 1);
				}
			}
			//If the Current particle is alive.
			if (particleComp->getParticleAlive(i))
			{
				//Checks to see if the Particle should be killed
				if (particleComp->checkParticlePastKillTime(i))
				{
					particleComp->killParticle(i);
				}
				//Updates all the living particles. Moving them and incrementing their timers.
				particleComp->updateParticle(i);

			}
		}
		//Increment the placeTimer. This value is set by the particlesPerSecond variable. When the place timer is more than 1 a particle is placed.
		particleComp->setPlaceParticleTimer(particleComp->getPlaceParticleTimer() + particleComp->getParticlesPerSecond());
	}
}

glm::vec2 ParticleSystem::randomDirectionVectorInRange(float t_angle, float t_angleOffset)
{
	std::random_device randomSeed; // obtain a random number from hardware
	std::mt19937 randomGenerator(randomSeed()); // seed the generator
	std::uniform_int_distribution<> randomDistribution(-t_angleOffset, t_angleOffset); // define the range
	int tempAngle = randomDistribution(randomGenerator);
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

