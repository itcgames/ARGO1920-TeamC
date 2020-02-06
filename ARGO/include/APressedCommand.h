#pragma once
#include "Command.h"
class APressedCommand :
	public Command
{
public:
	virtual void execute() { std::cout << "A Pressed Command" << std::endl; }
};

