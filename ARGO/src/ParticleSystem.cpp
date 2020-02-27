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
	ColourComponent* colourComp = static_cast<ColourComponent*>(t_entity.getComponent(ComponentType::Colour));

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
					//Set's the particles movement amd values, places it and decrements the placeTimer
					particleComp->setParticleColour(particleComp->getNextParticle(), randomColourBetweenTwoColours(colourComp));
					particleComp->setParticleSize(particleComp->getNextParticle(), randomFloatBetweenTwoValue(primtiveComp));
					particleComp->setParticleMovement(particleComp->getNextParticle(), randomDirectionVectorInRange(particleComp->getAngle(), particleComp->getAngleOffset(), particleComp->getSpeedOffset()));
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
///Generates a random unit vector within the range of angles. t_angle - t_angleoffset to  t_angle + t_angleoffset and scales it by the random scalar
///</summary>
glm::vec2 ParticleSystem::randomDirectionVectorInRange(float t_angle, float t_angleOffset, int t_offsetSpeed)
{
	//Converts the value into a percentile
	float tempSpeedScalar = 1 + (glm::linearRand(-t_offsetSpeed, t_offsetSpeed) / 100.0f);
	int tempAngle = glm::linearRand(-t_angleOffset, t_angleOffset);
	glm::vec2 tempSpeed = glm::vec2(glm::cos((t_angle + tempAngle) * M_PI / 180), glm::sin((t_angle + tempAngle) * M_PI / 180));
	return tempSpeed * tempSpeedScalar;
}

/// <summary>
/// Returns a colour between two colours
/// </summary>
/// <param name="t_colourComp"></param>
/// <returns></returns>
glm::uvec4 ParticleSystem::randomColourBetweenTwoColours(ColourComponent* t_colourComp)
{

	Colour primaryColour = t_colourComp->getColour();
	Colour secondaryColour = t_colourComp->getSecondaryColour();

	int redValue;
	int greenValue;
	int blueValue;

	//Random Range Function need the smaller number before the larger. 0 -255 works. 255-0 does not. The statements below prevent this error.
	//Red Value
	if (primaryColour.red < secondaryColour.red)
	{
		redValue = glm::linearRand((int)primaryColour.red, (int)secondaryColour.red);
	}
	else
	{
		redValue = glm::linearRand((int)secondaryColour.red, (int)primaryColour.red);
	}
	//Green Value
	if (primaryColour.green < secondaryColour.green)
	{
		greenValue = glm::linearRand((int)primaryColour.green, (int)secondaryColour.green);
	}
	else
	{
		greenValue = glm::linearRand((int)secondaryColour.green, (int)primaryColour.green);
	}
	//Blue Value
	if (primaryColour.blue < secondaryColour.blue)
	{
		blueValue = glm::linearRand((int)primaryColour.blue, (int)secondaryColour.blue);
	}
	else
	{
		blueValue = glm::linearRand((int)secondaryColour.blue, (int)primaryColour.blue);
	}

	return glm::uvec4{ redValue,greenValue ,blueValue, 255 };
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

float ParticleSystem::randomFloatBetweenTwoValue(PrimitiveComponent* t_primComp)
{
	return glm::linearRand(t_primComp->getMinimumSize(), t_primComp->getMaximumSize());
}

