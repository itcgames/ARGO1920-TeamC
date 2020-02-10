#pragma once
#include "Command.h"
class MoveUpCommand :
	public Command
{
public: 

	virtual void execute() { std::cout << "Move Up" << std::endl; }
};

