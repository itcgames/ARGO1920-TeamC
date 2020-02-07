#pragma once
#include "Command.h"
class XPressedCommand :
	public Command
{
public:
	virtual void execute() { std::cout << "X Pressed Command" << std::endl; }
};

