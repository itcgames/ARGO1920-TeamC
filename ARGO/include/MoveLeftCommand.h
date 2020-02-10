#pragma once
#include "Command.h"
class MoveLeftCommand :
	public Command
{
public:

	virtual void execute() { std::cout << "Move Left" << std::endl; }
};

