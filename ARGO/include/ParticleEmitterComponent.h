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

	void setParticle(glm::vec2 t_pos);//Sets a particle to a position if it isn't alive.
	void killParticle(int t_index);//Kills the particle at the given index
	void setEmitting(bool t_emitting);//Sets if the Emitter is emitting particles or not.
	bool getEmitting();
	int getMaxParticles();//Gets max particles. You set MAX particles on Component Creation
	int getNextParticle();//The Particle in the vector that will be placed next.
	void setNextParticle(int t_nextIndex);
	void setTimeToKillParticle(int t_time);//The Time in frames until a particle should die. 1 second = 60 frames. 150 = 2.5 seconds.
	int  getOffset();//Gets the offset from emitter location the particles should be placed.
	void setOffset(int t_offset);
	void setParticleMovement(int t_index, glm::vec2 t_moveVector);//Sets the movement variable of the particle at the given index
	glm::vec2 getParticleMovement(int t_index);
	void setPosition(glm::vec2 t_pos);//Sets the position of the Emitter. The point from which the particles emerge from
	glm::vec2 getEmitterPosition();
	bool isParticleDead(int t_index);//Compares the m_timeToKillParticle and the particle at the given index's timer. Returns true if the Particle should be killed, false if not.
	void updateParticle(int t_index);//Moves the Particle position using its speed. Increments it's timer.
	bool getParticleAlive(int t_index);//Returns the m_alive bool of the particle at the given index.
	void setParticlesPerSecond(float t_particlesPerSecond);//Pass in the number of particles per second desired to be released from the emitter.
	float getParticlesPerSecond();
	glm::vec2 getParticlePosition(int t_index);
	float getAngle();
	void setAngle(float t_angle);//Sets the centre angle of the Emitter. The AngleOffsets are  +/- deviations on this.
	float getAngleOffset();
	void setAngleOffset(float t_angleOffset);//Angle Offsets from m_angle. 
	float getPlaceParticleTimer();
	void setPlaceParticleTimer(float t_particleTimer);//The place timer is used to time when to place a particle.It is incremented by the m_particlesPerSecond variable per frame. Whenever if it is over 1 a particle is placed
	void setRotating(bool t_rotating);//Sets the Emitter to Rotate
	bool getRotating();
	void setRotatingSpeed(float t_rotateAnglePerFrame);//If the Emitter is set to rotate, It rotates at this rate
	float getRotatingSpeed();

private:
	std::vector<Particle> m_particles;//The Vector for Particles
	bool m_emitting{ true };//Tracks if an Emitter is putting out particles or not.
	int m_maxParticles{ 120 };
	int m_placeParticle{ 0 };//Tracks which Particle to place next. Loops when maxed
	int m_offsetAmount{ 10 };// -10 to 10 from placed position
	int m_timeToKillParticle{ 60 };//Time until killing a particle
	glm::vec2 m_position;
	float m_particlesPerSecond{ 60.0f / 60.0f };//The Particles 
	float m_angle = 10.0f;//The Centre of the angle particles will be fired down
	float m_angleOffset = 30.0f;//The Offset of the m_angle particles will be fired down. +5 or -5 by default.
	float m_speed = 8.0f;//The Speed of the Particles. Multiplied with a unit vector
	float m_placeParticleTimer{ 0 };//Tracks how long until the next particle is placed.
	bool m_rotating{ false };
	float m_rotateAnglePerFrame{ 0 };
};

