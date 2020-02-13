#include "Event.h"
#include "stdafx.h"

size_t BaseEvent::getNextEventID()
{
	static size_t eventId = 0;
	return eventId++;
} 