#include "stdafx.h"
#include "MenuScreen.h"

MenuScreen::MenuScreen(EventManager& t_eventManager, CommandSystem& t_commandSystem, InputSystem& t_inputSystem, RenderSystem& t_renderSystem) :
	m_eventManager{ t_eventManager }, 
	m_currentButton{ MenuButtonType::Play },
	m_commandSystem{ t_commandSystem },
	m_inputSystem{ t_inputSystem },
	m_renderSystem{ t_renderSystem }
{	
}

MenuScreen::~MenuScreen()
{
}

void MenuScreen::update(float t_deltaTime)
{
	m_inputSystem.update(m_inputEntity);
	m_commandSystem.update(m_inputEntity, m_eventManager);
}

void MenuScreen::reset()
{
	m_screenActive = true;
	m_currentButton = MenuButtonType::Play;
	for (Entity& menuButton : m_menuButtons)
	{
		updateButtonColour(menuButton, Utilities::MENU_BUTTON_DEFAULT_COLOUR);
	}
	m_renderSystem.setFocus(glm::vec2(Utilities::SCREEN_WIDTH / 2.0f, Utilities::SCREEN_HEIGHT / 2.0f));
	updateButtonColour(m_menuButtons[static_cast<int>(m_currentButton)], Utilities::MENU_BUTTON_HIGHLIGHTED_COLOUR);
}

void MenuScreen::render(SDL_Renderer* t_renderer)
{
	m_renderSystem.render(t_renderer, m_background);
	for (Entity& menuButton : m_menuButtons)
	{
		m_renderSystem.render(t_renderer, menuButton);
	}
}

void MenuScreen::initialise(SDL_Renderer* t_renderer, Controller& t_controller)
{
	m_screenActive = true;
	setControllerButtonMaps();

	m_background.addComponent(new VisualComponent("Menu_Background.png", t_renderer));
	m_background.addComponent(new TransformComponent(0, 0));

	m_menuButtons[static_cast<int>(MenuButtonType::Play)].addComponent(new VisualComponent("Play_Button.png", t_renderer));
	m_menuButtons[static_cast<int>(MenuButtonType::Help)].addComponent(new VisualComponent("Help_Button.png", t_renderer));
	m_menuButtons[static_cast<int>(MenuButtonType::Credits)].addComponent(new VisualComponent("Credits_Button.png", t_renderer));
	m_menuButtons[static_cast<int>(MenuButtonType::Quit)].addComponent(new VisualComponent("Quit_Button.png", t_renderer));
	m_menuButtons[static_cast<int>(MenuButtonType::Achievements)].addComponent(new VisualComponent("Achievements_Button.png", t_renderer));


	for (int index = 0; index < NUMBER_OF_MENU_BUTTONS; index++)
	{
		VisualComponent* visualComp = static_cast<VisualComponent*>(m_menuButtons[index].getComponent(ComponentType::Visual));
		createMenuButton(m_menuButtons[index], glm::vec2(Utilities::SCREEN_WIDTH / 2.0f - visualComp->getWidth() / 2.0f, (Utilities::SCREEN_HEIGHT / (NUMBER_OF_MENU_BUTTONS + 1)) * (index + 1)));
	}

	m_renderSystem.setFocus(glm::vec2(Utilities::SCREEN_WIDTH / 2.0f, Utilities::SCREEN_HEIGHT / 2.0f));
	updateButtonColour(m_menuButtons[static_cast<int>(m_currentButton)], Utilities::MENU_BUTTON_HIGHLIGHTED_COLOUR);

	createInputEntity(t_controller);

	m_eventManager.subscribeToEvent<MenuMoveBetweenUI>(std::bind(&MenuScreen::changeCurrentSelectedButton, this, std::placeholders::_1));
	m_eventManager.subscribeToEvent<MenuButtonPressed>(std::bind(&MenuScreen::buttonPressed, this, std::placeholders::_1));


} 

void MenuScreen::setControllerButtonMaps()
{
	using ButtonCommandPair = std::pair<ButtonType, Command*>;
	m_controllerButtonMaps[static_cast<int>(ButtonState::Pressed)] =
	{
		ButtonCommandPair(ButtonType::DpadUp, new MenuMoveUpCommand()),
		ButtonCommandPair(ButtonType::DpadDown, new MenuMoveDownCommand()),
		ButtonCommandPair(ButtonType::A, new MenuSelectButtonCommand())
	};
	m_controllerButtonMaps[static_cast<int>(ButtonState::Held)];
	m_controllerButtonMaps[static_cast<int>(ButtonState::Released)];
}

void MenuScreen::createMenuButton(Entity& t_menuButton, glm::vec2 t_position)
{
	updateButtonColour(t_menuButton, Utilities::MENU_BUTTON_DEFAULT_COLOUR);
	t_menuButton.addComponent(new TransformComponent(t_position));
}

void MenuScreen::createInputEntity(Controller& t_controller)
{
	m_inputEntity.addComponent(new InputComponent(t_controller,
		m_controllerButtonMaps[static_cast<int>(ButtonState::Pressed)],
		m_controllerButtonMaps[static_cast<int>(ButtonState::Held)],
		m_controllerButtonMaps[static_cast<int>(ButtonState::Released)]));
	m_inputEntity.addComponent(new CommandComponent());
}

void MenuScreen::changeCurrentSelectedButton(const MenuMoveBetweenUI& t_event)
{
	if (m_screenActive)
	{
		int currentButtonIndex = static_cast<int>(m_currentButton);
		updateButtonColour(m_menuButtons[currentButtonIndex], Utilities::MENU_BUTTON_DEFAULT_COLOUR);
		if (MoveDirection::Up == t_event.direction)
		{
			currentButtonIndex--;
		}
		else if (MoveDirection::Down == t_event.direction)
		{
			currentButtonIndex++;
		}
 		if (currentButtonIndex < 0)
		{
			currentButtonIndex = static_cast<int>(MenuButtonType::Quit);
		}
		else if (currentButtonIndex > NUMBER_OF_MENU_BUTTONS - 1)
		{
			currentButtonIndex = static_cast<int>(MenuButtonType::Play);
		}
		updateButtonColour(m_menuButtons[currentButtonIndex], Utilities::MENU_BUTTON_HIGHLIGHTED_COLOUR);
		m_currentButton = static_cast<MenuButtonType>(currentButtonIndex);
	}
}

void MenuScreen::buttonPressed(const MenuButtonPressed& t_event)
{
	if (m_screenActive && ButtonType::A == t_event.buttonPressed)
	{
		MenuStates newScreen = MenuStates::MainMenu;
		switch (m_currentButton)
		{
		case MenuButtonType::Play:
			newScreen = MenuStates::Game;
			break;
		case MenuButtonType::Help:
			newScreen = MenuStates::Help;
			break;
		case MenuButtonType::Credits:
			newScreen = MenuStates::Credits;
			break;
		case MenuButtonType::Achievements:
			newScreen = MenuStates::Achievements;
			break;
		case MenuButtonType::Quit:
			m_eventManager.emitEvent(CloseWindow());
			break;
		default:
			break;
		}
		if (MenuStates::MainMenu != newScreen)
		{
			m_screenActive = false;
			m_eventManager.emitEvent(ChangeScreen{ newScreen });
		}
	}
}

void MenuScreen::updateButtonColour(Entity& t_menuButton, glm::vec3 t_colour)
{ 
	VisualComponent* visualComp = static_cast<VisualComponent*>(t_menuButton.getComponent(ComponentType::Visual));
	visualComp->setColor(t_colour.x, t_colour.y, t_colour.z);
}

