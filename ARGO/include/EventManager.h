#pragma once
#include "Event.h"

class EventManager
{
public:
	template<class EventType>
	using call_type = std::function<void(const EventType&)>;

	// event type is equivelent to type T
	template <typename EventType>
	void subscribe(call_type<EventType> t_function)
	{
		size_t id = Event<EventType>::getId();
		if (id >= m_subscribers.size())
		{
			m_subscribers.resize(id + 1);
		}
		m_subscribers[id].push_back(CallbackWrapper<EventType>(t_function));
	}

	template <typename EventType>
	void emit(const EventType& t_event)
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
	struct CallbackWrapper
	{
		CallbackWrapper(call_type<EventType> t_callable) : m_callable(t_callable) {}
		void operator() (const BaseEvent& t_event)
		{
			m_callable(static_cast<const Event<EventType>&>(t_event).m_event);
		}
		call_type<EventType> m_callable;
	};
 

private:
	std::vector<std::vector<call_type<BaseEvent>>> m_subscribers;
};

