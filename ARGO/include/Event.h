#pragma once
#include "Entity.h" 
struct PhysicsMove
{
	glm::vec2 m_velocity;
	Entity& m_entity;
};

struct CloseWindow
{
};

class BaseEvent
{
protected:
	static size_t getNextEventID();
};

template <typename EventType>
class Event : public BaseEvent
{
public:
	static size_t getId()
	{
		static size_t id = BaseEvent::getNextEventID();
		return id;
	}
	Event(const EventType& t_event) : m_event(t_event) {}
	const EventType& m_event;
};