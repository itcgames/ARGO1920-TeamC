#pragma once
#include "Event.h"

class EventManager
{
public: 

	template <typename EventType>
	void subscribeToEvent(std::function<void(const EventType&)> t_function)
	{
		size_t id = Event<EventType>::getId();
		if (id >= m_subscribers.size())
		{
			m_subscribers.resize(id + 1);
		}
		m_subscribers[id].push_back(EventTypeWrapper<EventType>(t_function));
	}

	template <typename EventType>
	void emitEvent(const EventType& t_event)
	{
		size_t id = Event<EventType>::getId();
		if (id >= m_subscribers.size())
		{
			m_subscribers.resize(id + 1);
		}
		Event<EventType> eventWrapper(t_event);
		for (auto& receiver : m_subscribers[id])
		{
			receiver(eventWrapper);
		}
	}

	template <typename EventType>
	struct EventTypeWrapper
	{
		EventTypeWrapper(std::function<void(const EventType&)> t_callable) : m_callable(t_callable) {}
		void operator() (const BaseEvent& t_event)
		{
			m_callable(static_cast<const Event<EventType>&>(t_event).m_event);
		}
		std::function<void(const EventType&)> m_callable;
	};
 

private:
	// vector of functions that are subsribed to an event
	std::vector<std::vector<std::function<void(const BaseEvent&)>>> m_subscribers;
};

 
