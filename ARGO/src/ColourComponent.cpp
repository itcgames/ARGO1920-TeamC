#include "stdafx.h"
#include "ColourComponent.h"

ColourComponent::ColourComponent() :
	Component(ComponentType::Colour)
{
}

ColourComponent::ColourComponent(Uint8 t_red, Uint8 t_green, Uint8 t_blue, Uint8 t_alpha) :
	Component(ComponentType::Colour)
{
	m_colour.red = t_red;
	m_colour.green = t_green;
	m_colour.blue = t_blue;
	m_colour.alpha = t_alpha;
}

ColourComponent::ColourComponent(Colour t_colour) :
	Component(ComponentType::Colour)
{
	m_colour = t_colour;
}

ColourComponent::~ColourComponent()
{
}

void ColourComponent::setColor(Uint8 t_red, Uint8 t_green, Uint8 t_blue, Uint8 t_alpha)
{
	m_colour.red = t_red;
	m_colour.green = t_green;
	m_colour.blue = t_blue;
	m_colour.alpha = t_alpha;

	m_secondaryColour.red = 0;
	m_secondaryColour.green = 0;
	m_secondaryColour.blue = 0;
	m_secondaryColour.alpha = 0;
}

void ColourComponent::setColor(Colour t_colour)
{
	m_colour = t_colour;
}

void ColourComponent::setSecondaryColour(Colour t_colour)
{
	m_secondaryColour = t_colour;
}

void ColourComponent::setRed(Uint8 t_red)
{
	m_colour.red = t_red;
}

void ColourComponent::setGreen(Uint8 t_green)
{
	m_colour.green = t_green;
}

void ColourComponent::setBlue(Uint8 t_blue)
{
	m_colour.blue = t_blue;
}

void ColourComponent::setAlpha(Uint8 t_alpha)
{
	m_colour.alpha = t_alpha;
}

Colour ColourComponent::getColour()
{
	return m_colour;
}

Colour ColourComponent::getSecondaryColour()
{
	return m_secondaryColour;
}

Uint8 ColourComponent::getRed()
{
	return m_colour.red;
}

Uint8 ColourComponent::getGreen()
{
	return m_colour.green;
}

Uint8 ColourComponent::getBlue()
{
	return m_colour.blue;
}

Uint8 ColourComponent::getAlpha()
{
	return m_colour.alpha;
}
