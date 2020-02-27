#pragma once
#include <iostream>

///@Author Sean Abner Nash
///@Login C00217019

class Particle
{
public:
	Particle();
	~Particle();
	void setAlive(bool t_alive);
	bool getAlive();
	void setTimer(float t_timer);
	float getTimer();
	void setPosition(glm::vec2 t_pos);
	glm::vec2 getPosition();
	void setMovement(glm::vec2 t_move);
	glm::vec2 getMovement();
	void setColour(glm::uvec4 t_colour);
	glm::uvec4 getColour();
	void setSize(float t_size);
	float getSize();
private:
	float m_timeOut{ 0 };//This variable tracks how long a particle has been alive for
	bool m_alive{ false };
	glm::vec2 m_position;
	glm::vec2 m_movement{ 0,0 };
	glm::uvec4 m_colour{ 0,0,0,0 };
	float m_size{ 4 };

};

