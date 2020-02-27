#pragma once
#include "Entity.h" 
#include "Utilities.h"
#include "Controller.h"

namespace Events
{
	struct CreateBulletEvent
	{
		Entity& entity;
		glm::vec2 direction;
		float forceScale;
		int type;
		Controller& controller;
	};

	struct PhysicsMove
	{
		glm::vec2 velocity;
		Entity& entity;
	};

	struct PhysicsRotate
	{
		glm::vec2 rotation;
		Entity& entity;
	};

	struct MenuMoveBetweenUI 
	{
		MoveDirection direction;
	};

	struct MenuButtonPressed
	{
		ButtonType buttonPressed;
	};  

	struct CloseWindow
	{
	};

	struct ChangeScreen
	{
		MenuStates newScreen;
	};
}

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