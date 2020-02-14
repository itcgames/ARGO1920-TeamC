#include "stdafx.h"
#include "ParticleEmitterComponent.h"

ParticleEmitterComponent::ParticleEmitterComponent():
	Component(ComponentType::ParticleEmitter)
{
	for (int i = 0; i < m_maxParticles; i++)
	{
		Particle x;
		m_particles.push_back(x);
	}
}

ParticleEmitterComponent::ParticleEmitterComponent(glm::vec2 t_position, bool t_emitting, float t_angle, float t_angleOffset, float t_speed, int t_maxParticles, float t_particlePerSecond, int t_timeToKillParticle, bool t_rotating,float t_rotateAnglePerFrame) :
	Component(ComponentType::ParticleEmitter),
	m_position(t_position),
	m_angle(t_angle),
	m_angleOffset(t_angleOffset),
	m_speed(t_speed),
	m_maxParticles(t_maxParticles),
	m_timeToKillParticle(t_timeToKillParticle),
	m_emitting(t_emitting),
	m_particlesPerSecond(t_particlePerSecond/60.0f),
	m_rotating(t_rotating),
	m_rotateAnglePerFrame(t_rotateAnglePerFrame)
{
	for (int i = 0; i < m_maxParticles; i++)
	{
		Particle x;
		m_particles.push_back(x);
	}
}


void ParticleEmitterComponent::setParticle(glm::vec2 t_pos)
{
	if (m_particles[m_placeParticle].getAlive() == false)
	{
		m_particles[m_placeParticle].setPosition(t_pos);
		m_particles[m_placeParticle].setAlive(true);
		m_placeParticle++;
		
	}
	

	if (m_placeParticle >= m_maxParticles)
	{
		m_placeParticle = 0;
	}
}

void ParticleEmitterComponent::killParticle(int t_index)
{
	m_particles[t_index].setAlive(false);
}


void ParticleEmitterComponent::setEmitting(bool t_emitting)
{
	m_emitting = t_emitting;
}

bool ParticleEmitterComponent::getEmitting()
{
	return m_emitting;
}

void ParticleEmitterComponent::setMaxParticles(int t_maxParticle)
{
	m_maxParticles = t_maxParticle;
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

void ParticleEmitterComponent::setTimeToKillParticle(int t_time)
{
	m_timeToKillParticle = t_time;
}

int ParticleEmitterComponent::getOffset()
{
	return m_offsetAmount;
}

void ParticleEmitterComponent::setOffset(int t_offset)
{
	m_offsetAmount = t_offset;
}

void ParticleEmitterComponent::setParticleMovement(int t_index, glm::vec2 t_moveVector)
{
	m_particles[t_index].setMovement(t_moveVector);
}

glm::vec2 ParticleEmitterComponent::getParticleMovement(int t_index)
{
	return m_particles[t_index].getMovement();
}

void ParticleEmitterComponent::setPosition(glm::vec2 t_pos)
{
	m_position = t_pos;
}

bool ParticleEmitterComponent::isParticleDead(int t_index)
{
	if (m_particles[t_index].getTimer() > m_timeToKillParticle)
	{
		return true;
	}
	return false;
}

void ParticleEmitterComponent::updateParticle(int t_index)
{
	m_particles[t_index].setPosition(m_particles[t_index].getPosition() + (m_particles[t_index].getMovement())*m_speed);
	m_particles[t_index].setTimer(m_particles[t_index].getTimer() + 1);
}

bool ParticleEmitterComponent::getParticleAlive(int t_index)
{
	return m_particles[t_index].getAlive();
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

void ParticleEmitterComponent::setAngle(float t_angle)
{
	m_angle = t_angle;
}

float ParticleEmitterComponent::getAngleOffset()
{
	return m_angleOffset;
}

void ParticleEmitterComponent::setAngleOffset(float t_angleOffset)
{
	m_angleOffset = t_angleOffset;
}

float ParticleEmitterComponent::getPlaceParticleTimer()
{
	return m_placeParticleTimer;
}

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


