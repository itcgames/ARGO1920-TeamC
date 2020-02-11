#pragma once
#include "Component.h"

enum class EventType
{

};

class EventMessagingComponent :
	public Component
{
public:
	EventMessagingComponent();
	~EventMessagingComponent(); 
	void addEventMessage(std::pair<ComponentType, EventType> t_newEventMessage);
	void removeEventMessageByIndex(int t_indexToRemove);
	void removeEventMessagesByComponent(ComponentType t_componentType);
	std::vector<std::pair<ComponentType, EventType>> &getEventMessages();
	void setEventMessages(std::vector<std::pair<ComponentType, EventType>> t_eventMessages);
private:
	std::vector < std::pair<ComponentType, EventType>> m_eventMessages;
};

