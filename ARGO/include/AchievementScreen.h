#pragma once
#include "EventManager.h"
#include "Controller.h"
#include "InputSystem.h"
#include "CommandSystem.h"
#include "RenderSystem.h"

enum class CheckBoxParts
{
	Box,
	Check
};

enum class AchievmentParts
{
	Box,
	MainText,
	SubText
};

enum class AchievmentsType
{
	GameWon,
	EnemiesKilled,
	Hidden
};

class AchievementScreen
{
public:
	AchievementScreen(EventManager& t_eventManager, CommandSystem& t_commandSystem, InputSystem& t_inputSystem, RenderSystem& t_renderSystem);
	~AchievementScreen();

	void update(float t_deltaTime);
	void render(SDL_Renderer* t_renderer);
	void reset();
	void initialise(SDL_Renderer* t_renderer, Controller& t_controller);
private:
	EventManager& m_eventManager;
 
	void setControllerButtonMaps();

	void createBackground(SDL_Renderer* t_renderer);
	void createInputEntity(Controller& t_controller);
	void createAchievementEntities(SDL_Renderer* t_renderer);
	void createCheckBoxEntities(SDL_Renderer* t_renderer);

	void buttonPressed(const MenuButtonPressed& t_event);

	void updateAchievement(const UpdateAchievement& t_event);
	void handleAchievements(int t_enemiesKilled, int t_gamesWon);
	void checkStats();

	InputSystem& m_inputSystem;
	CommandSystem& m_commandSystem;
	RenderSystem& m_renderSystem;

	bool m_screenActive = false;

	ButtonCommandMap m_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS];

	static const int NUMBER_OF_CHECKBOX_PARTS = 2;
	static const int NUMBER_OF_ACHV_PARTS = 3;
	static const int NUMBER_OF_ACHIEVEMENTS = 3;


	Entity m_checkboxes[NUMBER_OF_CHECKBOX_PARTS][NUMBER_OF_ACHIEVEMENTS];
	Entity m_achievementBoxes[NUMBER_OF_ACHV_PARTS][NUMBER_OF_ACHIEVEMENTS];
	Entity m_background;	
	Entity m_inputEntity;

	bool m_hasAchievements[NUMBER_OF_ACHIEVEMENTS];

	std::string m_hiddenAchvString;
	bool m_showHiddenAchv = false;

};