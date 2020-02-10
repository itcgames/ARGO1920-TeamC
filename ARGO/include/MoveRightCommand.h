#pragma once
#include "Command.h"
class MoveRightCommand :
	public Command
{
public:

	virtual void execute() { std::cout << "Move Right" << std::endl; }
};

