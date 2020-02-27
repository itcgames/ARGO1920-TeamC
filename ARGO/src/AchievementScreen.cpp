#include "stdafx.h"
#include "AchievementScreen.h"
 
AchievementScreen::AchievementScreen(EventManager& t_eventManager, CommandSystem& t_commandSystem, InputSystem& t_inputSystem, RenderSystem& t_renderSystem) :
	m_eventManager{ t_eventManager }
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
	//	m_eventManager.subscribeToEvent<MenuMoveButtons>(std::bind(&MenuScreen::changeCurrentSelectedButton, this, std::placeholders::_1));

	m_eventManager.subscribeToEvent<UpdateAchievement>(std::bind(&AchievementScreen::updateAchievement, this, std::placeholders::_1))
}

void AchievementScreen::updateAchievement(const UpdateAchievement& t_event)
{
}
