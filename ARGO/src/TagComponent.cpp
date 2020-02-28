#include "stdafx.h"
#include "TagComponent.h"

TagComponent::TagComponent(Tag t_tag) :
	Component(ComponentType::Tag),
	m_tag(t_tag)
{
}

Tag TagComponent::getTag()
{
	return m_tag;
}

void TagComponent::setTag(Tag t_tag)
{
	m_tag = t_tag;
}
