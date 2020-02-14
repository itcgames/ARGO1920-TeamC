#pragma once
#include "Component.h"
#include "Particle.h"
#include <vector>
class ParticleEmitterComponent :
	public Component
{
public:
	ParticleEmitterComponent();
	ParticleEmitterComponent(glm::vec2* t_position);
	void setParticle(glm::vec2 t_pos);//Sets a particle to a position
	void killParticle(int t_index);//Kills the particle at the given index
	int randomOffset();//Returns a random value set by the offsetAmount Value
	void setEmitting(bool t_emitting);
	bool getEmitting();
	void setMaxParticles(int t_maxParticle);
	int getMaxParticles();
	int getNextParticle();
	void setNextParticle(int t_nextIndex);
	void setTimeToKillParticle(int t_time);
	void setOffset(int t_offset);
	void setParticleMovement(int t_index, glm::vec2 t_moveVector);
	glm::vec2 getParticleMovement(int t_index);
	void setPosition(glm::vec2* t_pos);
	bool isParticleDead(int t_index);
	void updateParticle(int t_index);
	bool getParticleAlive(int t_index);
	glm::vec2* getEmitterPosition();
	float getParticlesPerSecond();
	glm::vec2 getParticlePosition(int t_index);
	float getAngle();
	float getAngleOffset();

private:
	std::vector<Particle> m_particles;
	bool m_emitting{ true };
	int m_maxParticles{ 100 };
	int m_placeParticle{ 0 };//Tracks which Particle to place next. Loops when maxed
	int m_offsetAmount{ 10 };// -10 to 10 from placed position
	int m_timeToKillParticle{ 300 };//Time until killing a particle
	glm::vec2* m_position;
	float m_particlesPerSecond{ 300.0f / 60.0f };
	float m_angle = 0.0f;//The Centre of the angle particles will be fired down
	float m_angleOffset = 180.0f;//The Offset of the m_angle particles will be fired down. +5 or -5 by default.
	float m_speed = 1.0f;
};

