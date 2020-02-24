#include "stdafx.h"
#include "AchievementScreen.h"

AchievementScreen::AchievementScreen(EventManager& t_eventManager, Controller& t_controller, SDL_Renderer* t_renderer) :
	m_eventManager{ t_eventManager },
	m_controller{ t_controller }
{
}

AchievementScreen::~AchievementScreen()
{
}

void AchievementScreen::update(float t_deltaTime)
{
}

void AchievementScreen::render(SDL_Renderer* t_renderer)
{
}

void AchievementScreen::reset()
{
}

void AchievementScreen::initialise()
{
}

void AchievementScreen::passSystems()
{
}
