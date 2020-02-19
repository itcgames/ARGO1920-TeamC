#include "stdafx.h"
#include "LicenseScreen.h"
#include "..\include\LicenseScreen.h"

LicenseScreen::LicenseScreen(EventManager& t_eventManager, Controller& t_controller, SDL_Renderer* t_renderer) :
	m_eventManager{ t_eventManager },
	m_controller{ t_controller }
{
}

LicenseScreen::~LicenseScreen()
{
}

void LicenseScreen::update(Uint16 t_deltaTime)
{
}

void LicenseScreen::render(SDL_Renderer* t_renderer)
{
}

void LicenseScreen::reset()
{
}
