#pragma once
#include "Component.h"
#include "Particle.h"
#include <vector>
class ParticleEmitterComponent :
	public Component
{
public:
	ParticleEmitterComponent();
	ParticleEmitterComponent(glm::vec2 t_position, bool t_emitting = true, float t_angle = 10,
		float t_angleOffset = 45.0f, float t_speed = 8, int t_maxParticles = 100,
		float t_particlesPerSecond = 60.0f, int t_timeToKillParticle = 150, bool t_rotating = false, float t_rotateAnglePerFrame = 0.05f);

	void setParticle(glm::vec2 t_pos);
	void killParticle(int t_index);
	void setEmitting(bool t_emitting);
	bool getEmitting();
	int getMaxParticles();
	int getNextParticle();
	void setNextParticle(int t_nextIndex);
	void setTimeToKillParticle(int t_time);
	int  getOffset();
	void setOffset(int t_offset);
	void setParticleMovement(int t_index, glm::vec2 t_moveVector);
	glm::vec2 getParticleMovement(int t_index);
	void setPosition(glm::vec2 t_pos);
	glm::vec2 getEmitterPosition();
	bool checkParticlePastKillTime(int t_index);
	void updateParticle(int t_index, float t_dt);
	bool getParticleAlive(int t_index);
	void setParticlesPerSecond(float t_particlesPerSecond);
	float getParticlesPerSecond();
	glm::vec2 getParticlePosition(int t_index);
	float getAngle();
	void setAngle(float t_angle);
	float getAngleOffset();
	void setAngleOffset(float t_angleOffset);
	float getPlaceParticleTimer();
	void setPlaceParticleTimer(float t_particleTimer);
	void setRotating(bool t_rotating);
	bool getRotating();
	void setRotatingSpeed(float t_rotateAnglePerFrame);
	float getRotatingSpeed();

private:
	//The Vector for Particles
	std::vector<Particle> m_particles;
	//Tracks if an Emitter is putting out particles or not.
	bool m_emitting{ true };
	int m_maxParticles{ 120 };
	//Tracks which Particle to place next. Loops when maxed
	int m_placeParticle{ 0 };
	// The amount a particle's initial position varies from Emitter position
	int m_offsetAmount{ 10 };
	//Time until killing a particle
	int m_timeToKillParticle{ 60 };
	glm::vec2 m_position;
	//The Particles per second released. 1/60 = 1 particle per second
	float m_particlesPerSecond{ 60.0f / 60.0f };
	//The Centre of the angle particles will be fired down
	float m_angle = 10.0f;
	//The Offset of the m_angle particles will be fired down. +5 or -5 by default.
	float m_angleOffset = 30.0f;
	//The Speed of the Particles. Multiplied with a unit vector
	float m_speed = 8.0f;
	//Tracks how long until the next particle is placed.
	float m_placeParticleTimer{ 0 };
	bool m_rotating{ false };
	float m_rotateAnglePerFrame{ 0 };
};

