#include "stdafx.h"
#include "OptionsScreen.h"

OptionsScreen::OptionsScreen(EventManager& t_eventManager, Controller& t_controller, SDL_Renderer* t_renderer) :
	m_eventManager{ t_eventManager },
	m_controller{ t_controller }
{
}

OptionsScreen::~OptionsScreen()
{
}

void OptionsScreen::update(float t_deltaTime)
{
}

void OptionsScreen::render(SDL_Renderer* t_renderer)
{
}

void OptionsScreen::reset()
{
}

void OptionsScreen::initialise()
{
}
