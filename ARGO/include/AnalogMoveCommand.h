#pragma once
#include "Command.h"
class AnalogMoveCommand :
	public Command
{
public:
	virtual void execute() { std::cout << "Analog Move" << std::endl; }
};

