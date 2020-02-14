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
ParticleEmitterComponent::ParticleEmitterComponent(glm::vec2* t_position):
	Component(ComponentType::ParticleEmitter)
{
	m_position = t_position;
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
	}
	m_placeParticle++;
	if (m_placeParticle >= m_maxParticles)
	{
		m_placeParticle = 0;
	}
}

void ParticleEmitterComponent::killParticle(int t_index)
{
	m_particles[t_index].setAlive(false);
	m_angle++;
}

int ParticleEmitterComponent::randomOffset()
{
	return (rand() % (m_offsetAmount * 2) + 1) - m_offsetAmount;
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

void ParticleEmitterComponent::setPosition(glm::vec2* t_pos)
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

glm::vec2* ParticleEmitterComponent::getEmitterPosition()
{
	;
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

float ParticleEmitterComponent::getAngleOffset()
{
	return m_angleOffset;
}


