#pragma once
#include "Command.h"
class YPressedCommand :
	public Command
{
public:
	virtual void execute() { std::cout << "Y Pressed Command" << std::endl; }
};

