#include "stdafx.h"
#include "ParticleSystem.h"
#include "ext.hpp"

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::update(Entity& t_entity, float t_dt)
{
	ParticleEmitterComponent* particleComp = static_cast<ParticleEmitterComponent*>(t_entity.getComponent(ComponentType::ParticleEmitter));
	PrimitiveComponent* primtiveComp = static_cast<PrimitiveComponent*>(t_entity.getComponent(ComponentType::Primitive));
	TransformComponent* transComp = static_cast<TransformComponent*>(t_entity.getComponent(ComponentType::Transform));

	if (particleComp && primtiveComp && transComp)
	{
		particleComp->setPosition(transComp->getPos());//Sets the Emitter to the entity's tranform position. This makes it move with mobile entities.
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
				particleComp->updateParticle(i, t_dt);

			}
		}
		//Increment the placeTimer. This value is set by the particlesPerSecond variable. When the place timer is more than 1 a particle is placed.
		particleComp->setPlaceParticleTimer(particleComp->getPlaceParticleTimer() + particleComp->getParticlesPerSecond() * t_dt);
	}
}
void ParticleSystem::update(Entity& t_entity)
{
}
///<summary>
///Generates a random unit vector within the range of angles. t_angle - t_angleoffset to  t_angle + t_angleoffset
///</summary>
glm::vec2 ParticleSystem::randomDirectionVectorInRange(float t_angle, float t_angleOffset)
{
	std::random_device randomSeed; // obtain a random number from hardware
	std::mt19937 randomGenerator(randomSeed()); // seed the generator
	std::uniform_int_distribution<> randomDistribution(-t_angleOffset, t_angleOffset); // define the range
	int tempAngle = randomDistribution(randomGenerator);
	return  glm::vec2(glm::cos((t_angle + tempAngle) * M_PI / 180), glm::sin((t_angle + tempAngle) * M_PI / 180));

}
///<summary>
///Returns a random value set by the offsetAmount Value 
///</summary>
int ParticleSystem::randomOffset(int t_offsetAmount)
{
	return (rand() % (t_offsetAmount * 2) + 1) - t_offsetAmount;
}
///<summary>
///Pass in an angle and a reference to a Particle Component to change the angle to the given angle. 
///</summary>
void ParticleSystem::adjustParticleAngles(float t_newAngle, ParticleEmitterComponent* t_partComp)
{
	t_partComp->setAngle(t_newAngle);
}

