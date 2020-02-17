#pragma once
#include "Entity.h" 

struct createBulletEvent
{
	Entity& entity;
	glm::vec2 direction;
	float forceScale;
	int type;
};

struct PhysicsMove
{
	glm::vec2 velocity;
	Entity& entity;
};

struct MenuMoveButtonsUpDown
{
	Entity& entity;
	// bool for if its moving down or up
	bool isMoveDown;
};

struct MenuSelectButton
{
	Entity& entity;

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