#pragma once
#include "Command.h"
class MoveDownCommand :
	public Command
{
public:

	virtual void execute() { std::cout << "Move Down" << std::endl; }
};

