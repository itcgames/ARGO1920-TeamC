#include "stdafx.h"
#include "CreditsScreen.h"

CreditsScreen::CreditsScreen(EventManager& t_eventManager, Controller& t_controller, SDL_Renderer* t_renderer) :
	m_eventManager{ t_eventManager },
	m_controller{ t_controller }
{
}

CreditsScreen::~CreditsScreen()
{
}

void CreditsScreen::update(Uint16 t_deltaTime)
{
}

void CreditsScreen::render(SDL_Renderer* t_renderer)
{
}

void CreditsScreen::reset()
{
}
