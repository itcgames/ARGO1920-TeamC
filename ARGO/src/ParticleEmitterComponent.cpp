#include "stdafx.h"
#include "ParticleEmitterComponent.h"

ParticleEmitterComponent::ParticleEmitterComponent() :
	Component(ComponentType::ParticleEmitter)
{
	for (int i = 0; i < m_maxParticles; i++)
	{
		Particle particle;
		m_particles.push_back(particle);
	}
}
/// <summary>
///Constructor Function.
/// </summary>
ParticleEmitterComponent::ParticleEmitterComponent(glm::vec2 t_position, bool t_emitting, float t_angle, float t_angleOffset, float t_speed, int t_maxParticles, float t_particlesPerSecond, int t_timeToKillParticle, bool t_rotating, float t_rotateAnglePerFrame, EmitterType t_emitterType) :
	Component(ComponentType::ParticleEmitter),
	m_position(t_position),
	m_angle(t_angle),
	m_angleOffset(t_angleOffset),
	m_speed(t_speed),
	m_maxParticles(t_maxParticles),
	m_timeToKillParticle(t_timeToKillParticle),
	m_emitting(t_emitting),
	m_particlesPerSecond(t_particlesPerSecond / 60.0f),
	m_rotating(t_rotating),
	m_rotateAnglePerFrame(t_rotateAnglePerFrame),
	m_emitterType(t_emitterType)
{
	for (int i = 0; i < m_maxParticles; i++)
	{
		Particle particle;
		m_particles.push_back(particle);
	}
}
ParticleEmitterComponent::ParticleEmitterComponent(bool t_emitting, float t_speedOffset, float t_speed, int t_maxParticles, int t_timeToKillParticle, float t_particlesPerSecond) :
	Component(ComponentType::ParticleEmitter),
	m_position(glm::vec2(-100, -100)),
	m_angle(90.0f),
	m_angleOffset(180.0f),
	m_speed(t_speed),
	m_maxParticles(t_maxParticles),
	m_timeToKillParticle(t_timeToKillParticle),
	m_emitting(t_emitting),
	m_particlesPerSecond(t_particlesPerSecond / 60.0f),
	m_speedOffset(t_speedOffset),
	m_emitterType(EmitterType::Burst)
{
	for (int i = 0; i < m_maxParticles; i++)
	{
		Particle particle;
		m_particles.push_back(particle);
	}
}
/// <summary>
///This Function Places Particles. Passing in a glm::vec2 Position will have it place the currently queued particle if possible. It will skip over it if it isn't.
/// </summary>
void ParticleEmitterComponent::setParticle(glm::vec2 t_pos)
{
	if (m_particles[m_placeParticle].getAlive() == false)//If the current particle to be tracked is dead then it places it then increments the tracker
	{
		m_particles[m_placeParticle].setPosition(t_pos);
		m_particles[m_placeParticle].setAlive(true);
		m_placeParticle++;

	}


	if (m_placeParticle >= m_maxParticles)//Flips the tracker once it overflows.
	{
		m_placeParticle = 0;
		if (m_emitterType == EmitterType::Burst)
		{
			m_emitting = false;
		}
	}
}
///Kills a particle at a given index. Killed particles are not rendered, updated or moved. They will be reused once the queue reaches them again.
void ParticleEmitterComponent::killParticle(int t_index)
{
	m_particles[t_index].setAlive(false);
}

///Sets if the Emitter is emitting particles or not.
void ParticleEmitterComponent::setEmitting(bool t_emitting)
{
	m_emitting = t_emitting;
}

bool ParticleEmitterComponent::getEmitting()
{
	return m_emitting;
}

int ParticleEmitterComponent::getMaxParticles()
{
	return m_maxParticles;
}

int ParticleEmitterComponent::getNextParticle()
{
	return m_placeParticle;
}

void ParticleEmitterComponent::setNextParticle(int t_nextIndex)
{
	m_placeParticle = t_nextIndex;
}
/// <summary>
///The Time in frames until a particle should die. 1 second = 60 frames. 150 = 2.5 seconds.
/// </summary>
void ParticleEmitterComponent::setTimeToKillParticle(int t_time)
{
	m_timeToKillParticle = t_time;
}
///<summary>
///Gets the offset from emitter location the particles should be placed.
///</summary>
int ParticleEmitterComponent::getOffset()
{
	return m_offsetAmount;
}

void ParticleEmitterComponent::setOffset(int t_offset)
{
	m_offsetAmount = t_offset;
}
///<summary>
///Sets the movement variable of the particle at the given index
///</summary>
void ParticleEmitterComponent::setParticleMovement(int t_index, glm::vec2 t_moveVector)
{
	m_particles[t_index].setMovement(t_moveVector);
}

glm::vec2 ParticleEmitterComponent::getParticleMovement(int t_index)
{
	return m_particles[t_index].getMovement();
}
//Sets the position of the Emitter. The point from which the particles emerge from
void ParticleEmitterComponent::setPosition(glm::vec2 t_pos)
{
	m_position = t_pos;
}
///<summary>
///Compares the m_timeToKillParticle and the particle at the given index's timer. Returns true if the Particle should be killed, false if not.
///</summary>
bool ParticleEmitterComponent::checkParticlePastKillTime(int t_index)
{
	return m_particles[t_index].getTimer() > m_timeToKillParticle;
}
///<summary>
///Moves the Particle position using its speed. Increments it's timer.
///</summary>
void ParticleEmitterComponent::updateParticle(int t_index, float t_dt)
{
	m_particles[t_index].setPosition(m_particles[t_index].getPosition() + (m_particles[t_index].getMovement()) * m_speed * t_dt);
	m_particles[t_index].setTimer(m_particles[t_index].getTimer() + 1 * t_dt);
}

bool ParticleEmitterComponent::getParticleAlive(int t_index)
{
	return m_particles[t_index].getAlive();
}
///<summary>
///Pass in the number of particles per second desired to be released from the emitter.
///</summary>
void ParticleEmitterComponent::setParticlesPerSecond(float t_particlesPerSecond)
{
	m_particlesPerSecond = t_particlesPerSecond / 60.0f;
}

glm::vec2 ParticleEmitterComponent::getEmitterPosition()
{
	return m_position;
}

float ParticleEmitterComponent::getParticlesPerSecond()
{
	return m_particlesPerSecond;
}

glm::vec2 ParticleEmitterComponent::getParticlePosition(int t_index)
{
	return m_particles[t_index].getPosition();
}

float ParticleEmitterComponent::getAngle()
{
	return m_angle;
}
///<summary>
///Sets the centre angle of the Emitter. The AngleOffsets are  +/- deviations on this.
///</summary>
void ParticleEmitterComponent::setAngle(float t_angle)
{
	m_angle = t_angle;
}

float ParticleEmitterComponent::getAngleOffset()
{
	return m_angleOffset;
}
///<summary>
///Angle Offsets from m_angle. Particles will be fired down between the -angleOffset and +angleOffset 
///</summary>
void ParticleEmitterComponent::setAngleOffset(float t_angleOffset)
{
	m_angleOffset = t_angleOffset;
}
///<summary>
///The place timer is used to time when to place a particle.It is incremented by the m_particlesPerSecond variable per frame. Whenever if it is over 1 a particle is placed
///</summary>
float ParticleEmitterComponent::getPlaceParticleTimer()
{
	return m_placeParticleTimer;
}

///<summary>
//The place timer is used to time when to place a particle.It is incremented by the m_particlesPerSecond variable per frame. Whenever if it is over 1 a particle is placed
///</summary>
void ParticleEmitterComponent::setPlaceParticleTimer(float t_particleTimer)
{
	m_placeParticleTimer = t_particleTimer;
}

void ParticleEmitterComponent::setRotating(bool t_rotating)
{
	m_rotating = t_rotating;
}

bool ParticleEmitterComponent::getRotating()
{
	return m_rotating;
}

void ParticleEmitterComponent::setRotatingSpeed(float t_rotateAnglePerFrame)
{
	m_rotateAnglePerFrame = t_rotateAnglePerFrame;
}

float ParticleEmitterComponent::getRotatingSpeed()
{
	return m_rotateAnglePerFrame;
}

EmitterType ParticleEmitterComponent::getEmitterType()
{
	return m_emitterType;
}

void ParticleEmitterComponent::setEmitterType(EmitterType t_emitterType)
{
	m_emitterType = t_emitterType;
}

int ParticleEmitterComponent::getSpeedOffset()
{
	return m_speedOffset;
}

void ParticleEmitterComponent::setParticleColour(int t_index, glm::uvec4 t_colour)
{
	m_particles[t_index].setColour(t_colour);
}

glm::uvec4 ParticleEmitterComponent::getParticleColour(int t_index)
{
	return m_particles[t_index].getColour();
}

void ParticleEmitterComponent::setParticleSize(int t_index, float t_size)
{
	m_particles[t_index].setSize(t_size);
}

float ParticleEmitterComponent::getParticleSize(int t_index)
{
	return m_particles[t_index].getSize();
}

void ParticleEmitterComponent::setSpeed(float t_speed)
{
	m_speed = t_speed;
}



