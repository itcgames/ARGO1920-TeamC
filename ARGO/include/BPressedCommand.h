#pragma once
#include "Command.h"
class BPressedCommand :
	public Command
{
public:
	virtual void execute() { std::cout << "B Pressed Command" << std::endl; }
};

