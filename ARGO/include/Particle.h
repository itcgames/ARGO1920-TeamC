#pragma once
#include <iostream>
//#include <SFML/Graphics.hpp>

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
private:

	float m_timeOut{ 0 };
	bool m_alive{ false };
	glm::vec2 m_position;
	glm::vec2 m_movement{ 0,0 };

};

