#include "InputComponent.h"


InputComponent::InputComponent() :
	Component(ComponentType::Input)
{

}

InputComponent::~InputComponent()
{
	delete keystates;
}

void InputComponent::update()
{
	if (keystates[SDL_SCANCODE_UP])
	{
		m_keys.up = true;
	}
	else
	{
		m_keys.up = false;
	}

	if (keystates[SDL_SCANCODE_DOWN])
	{
		m_keys.down = true;
	}
	else
	{
		m_keys.down = false;
	}

	if (keystates[SDL_SCANCODE_LEFT])
	{
		m_keys.left = true;
	}
	else
	{
		m_keys.left = false;
	}

	if (keystates[SDL_SCANCODE_RIGHT])
	{
		m_keys.right = true;
	}
	else
	{
		m_keys.right = false;
	}
}
