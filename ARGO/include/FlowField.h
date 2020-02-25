#pragma once
#include "Utilities.h"
#include "Entity.h"

class FlowField
{
public:
	FlowField(Entity(&t_players)[Utilities::S_MAX_PLAYERS]);

private:
	Entity(&m_players)[Utilities::S_MAX_PLAYERS];
};

