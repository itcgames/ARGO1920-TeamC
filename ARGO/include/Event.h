#pragma once
#include "Entity.h" 
#include "Utilities.h"
#include "Controller.h"

struct CreateBulletEvent
{
	Entity& entity;
	glm::vec2 direction;
	float forceScale;
	int type;
	Controller& controller;
};

struct PickupGrabbed
{
	Entity* pickup;
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

struct UpdateAchievement
{
	int enemiesKilled = 0;
	int gamesWon = 0;
};

struct CloseWindow
{
};

struct ChangeScreen
{
	MenuStates newScreen;
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