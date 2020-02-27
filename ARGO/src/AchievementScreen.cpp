#include "stdafx.h"
#include "AchievementScreen.h"
#include "..\include\AchievementScreen.h"

AchievementScreen::AchievementScreen(EventManager& t_eventManager, CommandSystem& t_commandSystem, InputSystem& t_inputSystem, RenderSystem& t_renderSystem) :
	m_eventManager{ t_eventManager },
	m_commandSystem{ t_commandSystem },
	m_inputSystem{ t_inputSystem },
	m_renderSystem{ t_renderSystem }
{
}

AchievementScreen::~AchievementScreen()
{
}

void AchievementScreen::update(float t_deltaTime)
{
	m_inputSystem.update(m_inputEntity);
	m_commandSystem.update(m_inputEntity, m_eventManager);
}

void AchievementScreen::render(SDL_Renderer* t_renderer)
{
	m_renderSystem.render(t_renderer, m_background);
	for (int index = 0; index < NUMBER_OF_ACHIEVEMENTS; index++)
	{
		m_renderSystem.render(t_renderer, m_checkboxes[static_cast<int>(CheckBoxParts::Box)][index]);
		if (m_hasAchievements[index])
		{
			m_renderSystem.render(t_renderer, m_checkboxes[static_cast<int>(CheckBoxParts::Check)][index]);
		}
	}
	for (int index = 0; index < NUMBER_OF_ACHV_PARTS; index++)
	{
		for (int index2 = 0; index2 < NUMBER_OF_ACHIEVEMENTS; index2++)
		{
			m_renderSystem.render(t_renderer, m_achievementBoxes[index][index2]);
		}
	}
}

void AchievementScreen::reset()
{
	checkStats();
	m_screenActive = true;
	m_renderSystem.setFocus(glm::vec2(Utilities::SCREEN_WIDTH / 2.0f, Utilities::SCREEN_HEIGHT / 2.0f));
}

void AchievementScreen::initialise(SDL_Renderer* t_renderer, Controller& t_controller)
{
	checkStats();
	m_hiddenAchvString = "Win 10 Games and Kill 1000 Enemies";

	setControllerButtonMaps();


	for (bool& hasAchievement : m_hasAchievements)
	{
		hasAchievement = false;
	}

	m_screenActive = true;


	createBackground(t_renderer);
	createInputEntity(t_controller);
	createAchievementEntities(t_renderer);
	createCheckBoxEntities(t_renderer);



	m_renderSystem.setFocus(glm::vec2(Utilities::SCREEN_WIDTH / 2.0f, Utilities::SCREEN_HEIGHT / 2.0f));

	m_eventManager.subscribeToEvent<UpdateAchievement>(std::bind(&AchievementScreen::updateAchievement, this, std::placeholders::_1));
	m_eventManager.subscribeToEvent<MenuButtonPressed>(std::bind(&AchievementScreen::buttonPressed, this, std::placeholders::_1));

}

void AchievementScreen::setControllerButtonMaps()
{
	using ButtonCommandPair = std::pair<ButtonType, Command*>;
	m_controllerButtonMaps[static_cast<int>(ButtonState::Pressed)] =
	{
		ButtonCommandPair(ButtonType::B, new MenuCancelCommand()),
	};
	m_controllerButtonMaps[static_cast<int>(ButtonState::Held)];
	m_controllerButtonMaps[static_cast<int>(ButtonState::Released)];
}

void AchievementScreen::createBackground(SDL_Renderer* t_renderer)
{
	m_background.addComponent(new VisualComponent("Menu_Background.png", t_renderer));
	m_background.addComponent(new TransformComponent(0, 0));
}

void AchievementScreen::createInputEntity(Controller& t_controller)
{
	m_inputEntity.addComponent(new InputComponent(t_controller,
		m_controllerButtonMaps[static_cast<int>(ButtonState::Pressed)],
		m_controllerButtonMaps[static_cast<int>(ButtonState::Held)],
		m_controllerButtonMaps[static_cast<int>(ButtonState::Released)]));
	m_inputEntity.addComponent(new CommandComponent());
}

void AchievementScreen::createAchievementEntities(SDL_Renderer* t_renderer)
{
	std::string AchievmentStrings[NUMBER_OF_ACHIEVEMENTS][2]
	{
		{"Win a Game","The First Step"},
		{"Kill 50 Enemies","All Those Poor Defenseless Aliens"},
		{"??????????","Surpassing The Endless Horde"}
	};
	for (int index = 0; index < NUMBER_OF_ACHV_PARTS; index++)
	{
		m_achievementBoxes[static_cast<int>(AchievmentParts::Box)][index].addComponent(new VisualComponent((index == NUMBER_OF_ACHV_PARTS - 1) ? "AchvBoxHidden.png" : "AchvBox.png", t_renderer));
		m_achievementBoxes[static_cast<int>(AchievmentParts::Box)][index].addComponent(new TransformComponent(glm::vec2(Utilities::SCREEN_WIDTH * (1 / 20.0f), Utilities::SCREEN_HEIGHT / 4.0f * (index + 1))));
		VisualComponent* boxVisual = static_cast<VisualComponent*>(m_achievementBoxes[static_cast<int>(AchievmentParts::Box)][index].getComponent(ComponentType::Visual));
		glm::vec2 boxSize = glm::vec2(boxVisual->getWidth(), boxVisual->getHeight());
		glm::vec2 boxPos = static_cast<TransformComponent*>(m_achievementBoxes[static_cast<int>(AchievmentParts::Box)][index].getComponent(ComponentType::Transform))->getPos();

		m_achievementBoxes[static_cast<int>(AchievmentParts::MainText)][index].addComponent(new TextComponent("ariblk.ttf", t_renderer, Utilities::MEDIUM_FONT, true, (index == NUMBER_OF_ACHV_PARTS - 1 && m_showHiddenAchv) ? m_hiddenAchvString : AchievmentStrings[index][0], Utilities::UI_COLOUR.x, Utilities::UI_COLOUR.y, Utilities::UI_COLOUR.z));
		TextComponent* mainTextComp = static_cast<TextComponent*>(m_achievementBoxes[static_cast<int>(AchievmentParts::MainText)][index].getComponent(ComponentType::Text));
		glm::vec2 mainTextSize = glm::vec2(mainTextComp->getWidth(), mainTextComp->getHeight());

		m_achievementBoxes[static_cast<int>(AchievmentParts::MainText)][index].addComponent(new TransformComponent(glm::vec2(boxPos.x + (boxSize.x / 40.0f), boxPos.y + (boxSize.y / 3.0f) - mainTextSize.y / 2.0f)));


		m_achievementBoxes[static_cast<int>(AchievmentParts::SubText)][index].addComponent(new TextComponent("ariblk.ttf", t_renderer, Utilities::SMALL_FONT, true, AchievmentStrings[index][1], 0, 0, 0));
		TextComponent* subTextComp = static_cast<TextComponent*>(m_achievementBoxes[static_cast<int>(AchievmentParts::SubText)][index].getComponent(ComponentType::Text));
		glm::vec2 subTextSize = glm::vec2(subTextComp->getWidth(), subTextComp->getHeight());
		m_achievementBoxes[static_cast<int>(AchievmentParts::SubText)][index].addComponent(new TransformComponent(glm::vec2(boxPos.x + (boxSize.x / 40.0f), boxPos.y + (boxSize.y * 2.0f / 3.0f) - subTextSize.y / 2.0f)));
	}
}

void AchievementScreen::createCheckBoxEntities(SDL_Renderer* t_renderer)
{
	for (int index = 0; index < NUMBER_OF_ACHIEVEMENTS; index++)
	{
		m_checkboxes[static_cast<int>(CheckBoxParts::Box)][index].addComponent(new VisualComponent("Checkbox.png", t_renderer));
		m_checkboxes[static_cast<int>(CheckBoxParts::Box)][index].addComponent(new TransformComponent(glm::vec2(Utilities::SCREEN_WIDTH * (5.0f / 6.0f), Utilities::SCREEN_HEIGHT / 4.0f * (index + 1))));
		VisualComponent* boxVisual = static_cast<VisualComponent*>(m_checkboxes[static_cast<int>(CheckBoxParts::Box)][index].getComponent(ComponentType::Visual));
		glm::vec2 boxSize = glm::vec2(boxVisual->getWidth(), boxVisual->getHeight());
		glm::vec2 boxPos = static_cast<TransformComponent*>(m_checkboxes[static_cast<int>(CheckBoxParts::Box)][index].getComponent(ComponentType::Transform))->getPos();



		m_checkboxes[static_cast<int>(CheckBoxParts::Check)][index].addComponent(new VisualComponent("CheckMark.png", t_renderer));
		VisualComponent* checkVisual = static_cast<VisualComponent*>(m_checkboxes[static_cast<int>(CheckBoxParts::Check)][index].getComponent(ComponentType::Visual));
		glm::vec2 checkSize = glm::vec2(checkVisual->getWidth(), checkVisual->getHeight());



		m_checkboxes[static_cast<int>(CheckBoxParts::Check)][index].addComponent(new TransformComponent(boxPos + boxSize / 2.0f - checkSize / 2.0f));
 	}
}

void AchievementScreen::buttonPressed(const MenuButtonPressed& t_event)
{
	if (m_screenActive && ButtonType::B == t_event.buttonPressed)
	{
		m_screenActive = false;
		m_eventManager.emitEvent<ChangeScreen>(ChangeScreen{ MenuStates::MainMenu });
	}
}

void AchievementScreen::updateAchievement(const UpdateAchievement& t_event)
{
	std::string fullPath = SDL_GetBasePath();

	// get it to point at assets folder inside argo folder
	if (fullPath.find("\Debug") != std::string::npos)
	{
		fullPath.replace(fullPath.find("\Debug"), std::string("\Debug").length(), "\ARGO");
	}
	else if (fullPath.find("\Release") != std::string::npos)
	{
		fullPath.replace(fullPath.find("\Release"), std::string("\Release").length(), "\ARGO");

	}
	fullPath.append(Utilities::FILES_PATH + "achievements.txt");

	std::ifstream achievementsFileRead;
	achievementsFileRead.open(fullPath);

	std::string lineContents;

	int gamesWonValue = 0;
	int enemiesKilled = 0;

	if (achievementsFileRead.is_open())
	{
		while (std::getline(achievementsFileRead, lineContents))
		{
			if (lineContents.find("GamesWon:") != std::string::npos)
			{
				gamesWonValue = std::stoi(lineContents.substr(lineContents.find(":") + 1));
			}
			if (lineContents.find("EnemiesKilled:") != std::string::npos)
			{
				enemiesKilled = std::stoi(lineContents.substr(lineContents.find(":") + 1));
			}
		}
	}


	enemiesKilled += t_event.enemiesKilled;
	gamesWonValue += t_event.gamesWon;

	achievementsFileRead.close();


	std::ofstream achievementsFileWrite;
	achievementsFileWrite.open(fullPath);
	achievementsFileWrite.clear();
	achievementsFileWrite << "GamesWon:" + std::to_string(gamesWonValue) + "\n";
	achievementsFileWrite << "EnemiesKilled:" + std::to_string(enemiesKilled);
	achievementsFileWrite.close();


	handleAchievements(enemiesKilled, gamesWonValue);
}

void AchievementScreen::handleAchievements(int t_enemiesKilled, int t_gamesWon)
{
	if (!m_hasAchievements[static_cast<int>(AchievmentsType::EnemiesKilled)])
	{
		if (t_enemiesKilled >= 50)
		{
			m_hasAchievements[static_cast<int>(AchievmentsType::EnemiesKilled)] = true;
			Utilities::Achievements::numberOfUnlockedAchv++;
 		}
	}
	if (!m_hasAchievements[static_cast<int>(AchievmentsType::GameWon)])
	{
		if (t_gamesWon > 0)
		{
			m_hasAchievements[static_cast<int>(AchievmentsType::GameWon)] = true;
			Utilities::Achievements::numberOfUnlockedAchv++;

		}
	}
	if (!m_hasAchievements[static_cast<int>(AchievmentsType::Hidden)])
	{
		if (t_enemiesKilled >= 1000 && t_gamesWon >= 10)
		{
			m_hasAchievements[static_cast<int>(AchievmentsType::Hidden)] = true;
			Utilities::Achievements::numberOfUnlockedAchv++;

		}
	}
}

void AchievementScreen::checkStats()
{
	std::string fullPath = SDL_GetBasePath();

	// get it to point at assets folder inside argo folder
	if (fullPath.find("\Debug") != std::string::npos)
	{
		fullPath.replace(fullPath.find("\Debug"), std::string("\Debug").length(), "\ARGO");
	}
	else if (fullPath.find("\Release") != std::string::npos)
	{
		fullPath.replace(fullPath.find("\Release"), std::string("\Release").length(), "\ARGO");

	}
	fullPath.append(Utilities::FILES_PATH + "achievements.txt");

	std::ifstream achievementsFileRead;
	achievementsFileRead.open(fullPath);

	std::string lineContents;

	int gamesWonValue = 0;
	int enemiesKilled = 0;

	if (achievementsFileRead.is_open())
	{
		while (std::getline(achievementsFileRead, lineContents))
		{
			if (lineContents.find("GamesWon:") != std::string::npos)
			{
				gamesWonValue = std::stoi(lineContents.substr(lineContents.find(":") + 1));
			}
			if (lineContents.find("EnemiesKilled:") != std::string::npos)
			{
				enemiesKilled = std::stoi(lineContents.substr(lineContents.find(":") + 1));
			}
		}
	}
	handleAchievements(enemiesKilled, gamesWonValue);
}
