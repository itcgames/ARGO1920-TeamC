#include "stdafx.h"
#include "MenuScreen.h"
#include "..\include\MenuScreen.h"

MenuScreen::MenuScreen(SDL_Renderer* t_renderer, MenuStates& t_currentScreen, EventManager& t_eventManager, Controller t_controllers[Utilities::NUMBER_OF_PLAYERS], ButtonCommandMap t_controllerButtonMaps[Utilities::NUMBER_OF_CONTROLLER_MAPS][Utilities::NUMBER_OF_PLAYERS]) :
	m_renderer{ t_renderer },
	m_currentScreen{ t_currentScreen },
	m_eventManager{ t_eventManager },
	m_controllers{ *t_controllers },
	m_controllerButtonMaps{ **t_controllerButtonMaps }
{
	// create buttons

	m_menuButtons[static_cast<int>(MenuButtonType::Play)].addComponent(new VisualComponent("play_button_temp.png", m_renderer));
	m_menuButtons[static_cast<int>(MenuButtonType::Options)].addComponent(new VisualComponent("options_button_temp.png", m_renderer));
	m_menuButtons[static_cast<int>(MenuButtonType::Credits)].addComponent(new VisualComponent("credits_button_temp.png", m_renderer));
	m_menuButtons[static_cast<int>(MenuButtonType::Quit)].addComponent(new VisualComponent("quit_button_temp.png", m_renderer));
	m_menuButtons[static_cast<int>(MenuButtonType::Achievements)].addComponent(new VisualComponent("achv_button_temp.png", m_renderer));

	float middleOfScreenWidth = Utilities::SCREEN_WIDTH / 2.0f;
	float heightDistBetweenButtons = Utilities::SCREEN_HEIGHT / 6.0f;
	for (int index = 0; index < NUMBER_OF_MENU_BUTTONS; index++)
	{
		m_menuButtons[index].addComponent(new CommandComponent());
		m_menuButtons[index].addComponent(new TransformComponent(glm::vec2(middleOfScreenWidth, (index + 1) * heightDistBetweenButtons)));
		m_menuButtons[index].addComponent(new InputComponent(m_controllers[index],
			m_controllerButtonMaps[static_cast<int>(ButtonState::Pressed)][index],
			m_controllerButtonMaps[static_cast<int>(ButtonState::Held)][index],
			m_controllerButtonMaps[static_cast<int>(ButtonState::Released)][index]));
		m_menuButtons[index].addComponent(new TagComponent(Tag::MenuButton));
	}



	m_eventManager.subscribeToEvent<MenuMoveButtonsUpDown>(std::bind(&MenuScreen::changeCurrentSelectedButton, this, std::placeholders::_1));
	m_eventManager.subscribeToEvent<MenuSelectButton>(std::bind(&MenuScreen::currentButtonPressed, this, std::placeholders::_1));



}

MenuScreen::~MenuScreen()
{
}

void MenuScreen::update(bool t_canTick, bool t_canRender, Uint16 t_deltaTime)
{

	//update buttons
	for (Entity& menuButton : m_menuButtons)
	{
		if (t_canTick)
		{
			m_inputSystem.update(menuButton);
			m_commandSystem.update(menuButton);
		}
		if (t_canRender)
		{
			m_renderSystem.render(m_renderer, menuButton);
		}
	}




}

void MenuScreen::changeCurrentSelectedButton(const MenuMoveButtonsUpDown& t_event)
{
	int currentButtonIndex = static_cast<int>(m_currentButton);
	currentButtonIndex = t_event.isMoveDown ? currentButtonIndex + 1 : currentButtonIndex - 1;
	currentButtonIndex = glm::clamp(currentButtonIndex, 0, (NUMBER_OF_MENU_BUTTONS - 1));
	m_currentButton = static_cast<MenuButtonType>(currentButtonIndex);
}

void MenuScreen::currentButtonPressed(const MenuSelectButton& t_event)
{
	switch (m_currentButton)
	{
	case MenuButtonType::Play:
		m_currentScreen = MenuStates::Game;
		break;
	case MenuButtonType::Options:
		m_currentScreen = MenuStates::Options;
		break;
	case MenuButtonType::Credits:
		m_currentScreen = MenuStates::Credits;
		break;
	case MenuButtonType::Achievements:
		m_currentScreen = MenuStates::Achievements;
		break;
	case MenuButtonType::Quit:
		SDL_QUIT;
		break;
	default:
		break;
	}
}

