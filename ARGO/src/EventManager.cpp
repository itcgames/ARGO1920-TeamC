#include "stdafx.h"
#include "EventManager.h"
#include "..\include\EventManager.h"



size_t BaseEvent::getNextEventID()
{
	static size_t eventId = 0;
	return eventId++;
}
