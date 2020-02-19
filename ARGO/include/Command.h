#pragma once
#include "Command.h"
class Command
{
public:
	virtual void execute() = 0;
protected:
	Command() {};
};


class MoveRightCommand :
	public Command
{
public:

	virtual void execute() { std::cout << "Move Right" << std::endl; }
};

class AnalogMoveCommand :
	public Command
{
public:
	virtual void execute() { std::cout << "Analog Move" << std::endl; }
	glm::vec2 m_moveVelocity;
};

class MoveDownCommand :
	public Command
{
public:

	virtual void execute() { std::cout << "Move Down" << std::endl; }
};

class MoveLeftCommand :
	public Command
{
public:

	virtual void execute() { std::cout << "Move Left" << std::endl; }
};

class MoveUpCommand :
	public Command
{
public:

	virtual void execute() { std::cout << "Move Up" << std::endl; }
};

class CloseWindowCommand :
	public Command
{
public:
	virtual void execute() { std::cout << "Close Window" << std::endl; }
};

class FireBulletCommand :
	public Command
{
public:
	virtual void execute() { std::cout << "Fire Bullet" << std::endl; }
};

class MenuMoveUpCommand :
	public Command
{
public:
	virtual void execute() { std::cout << "Move Up Menu" << std::endl; }
};

class MenuMoveDownCommand :
	public Command
{
public:
	virtual void execute() { std::cout << "Move Down Menu" << std::endl; }
};

class MenuSelectButtonCommand :
	public Command
{
public:
	virtual void execute() { std::cout << "Button Pressed" << std::endl; }
};

