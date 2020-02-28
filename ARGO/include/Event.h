#pragma once
#include "Entity.h" 
#include "Utilities.h"
#include "Controller.h"
#include "WeaponComponent.h"

struct CreateBulletEvent
{
	Entity& entity;
	glm::vec2 direction;
	float forceScale;
	Weapon type;
	Controller& controller;
};

struct PickupGrabbed
{
	Entity* pickup;
};

struct CreateGlowStickEvent
{
	Entity& entity;
	glm::vec2 direction;
	float forceScale;
	Controller& controller;
};

struct PhysicsMove
{
	glm::vec2 velocity;
	Entity& entity;
};

struct EnemyKilled
{
	Entity* enemy;
};

struct PlayerKilled
{
	Entity* player;
};

struct Explosion
{
	glm::vec2 position;
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