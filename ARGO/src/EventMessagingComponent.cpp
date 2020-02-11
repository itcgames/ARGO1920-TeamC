#include "stdafx.h"
#include "EventMessagingComponent.h"
#include "..\include\EventMessagingComponent.h"

EventMessagingComponent::EventMessagingComponent():
	Component(ComponentType::EventMessaging)
{
}

EventMessagingComponent::~EventMessagingComponent()
{
}

void EventMessagingComponent::addEventMessage(std::pair<ComponentType, EventType> t_newEventMessage)
{
	m_eventMessages.push_back(t_newEventMessage);
}

void EventMessagingComponent::removeEventMessageByIndex(int t_indexToRemove)
{
	m_eventMessages.erase(m_eventMessages.begin() + t_indexToRemove);
}

void EventMessagingComponent::removeEventMessagesByComponent(ComponentType t_componentType)
{
}

//std::vector<std::pair<ComponentType, EventType>>& EventMessagingComponent::getEventMessages()
//{
//	// TODO: insert return statement here
//	return NULL;
//}

void EventMessagingComponent::setEventMessages(std::vector<std::pair<ComponentType, EventType>> t_eventMessages)
{
}
