#include "stdafx.h"
#include "Particle.h"

///@Author Sean Abner Nash
///@Login C00217019

Particle::Particle()
{

}

Particle::~Particle()
{


}


void Particle::setAlive(bool t_alive)
{
	m_alive = t_alive;
	m_timeOut = 0;
}


bool Particle::getAlive()
{
	return m_alive;
}

void Particle::setTimer(float t_timer)
{
	m_timeOut = t_timer;
}

float Particle::getTimer()
{
	return m_timeOut;
}
///<summary>
///Sets the position of the Emitter. The point from which the particles emerge from
///</summary>
void Particle::setPosition(glm::vec2 t_pos)
{
	m_position = t_pos;
}

glm::vec2 Particle::getPosition()
{
	return m_position;
}

void Particle::setMovement(glm::vec2 t_move)
{
	m_movement = t_move;
}

glm::vec2 Particle::getMovement()
{
	return m_movement;
}

void Particle::setColour(glm::uvec4 t_colour)
{
	m_colour = t_colour;
}

glm::uvec4 Particle::getColour()
{
	return m_colour;
}

void Particle::setSize(float t_size)
{
	m_size = t_size;
}

float Particle::getSize()
{
	return m_size;
}
