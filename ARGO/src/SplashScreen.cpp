#include "stdafx.h"
#include "SplashScreen.h"

SplashScreen::SplashScreen(EventManager& t_eventManager, Controller& t_controller, SDL_Renderer* t_renderer) :
	m_eventManager{ t_eventManager },
	m_controller{ t_controller }
{
}

SplashScreen::~SplashScreen()
{
}

void SplashScreen::update(Uint16 t_deltaTime)
{
}

void SplashScreen::render(SDL_Renderer* t_renderer)
{
}

void SplashScreen::reset()
{
}
