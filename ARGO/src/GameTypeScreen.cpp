#include "stdafx.h"
#include "GameTypeScreen.h"
#include "..\include\GameTypeScreen.h"

GameTypeScreen::GameTypeScreen(EventManager& t_eventManager, CommandSystem& t_commandSystem, InputSystem& t_inputSystem, RenderSystem& t_renderSystem) :
	m_eventManager{ t_eventManager },
	m_commandSystem{ t_commandSystem },
	m_inputSystem{ t_inputSystem },
	m_renderSystem{ t_renderSystem }
{
}

GameTypeScreen::~GameTypeScreen()
{
}

void GameTypeScreen::update(float t_deltaTime)
{
	m_inputSystem.update(m_inputEntity);
	m_commandSystem.update(m_inputEntity, m_eventManager);
}

void GameTypeScreen::reset()
{
	m_currentButton = GameTypeButtonType::Offline;
	for (Entity& gameTypeButton : m_gameTypeButtons)
	{
		updateButtonColour(gameTypeButton, Utilities::MENU_BUTTON_DEFAULT_COLOUR);
	}
	m_renderSystem.setFocus(glm::vec2(Utilities::SCREEN_WIDTH / 2.0f, Utilities::SCREEN_HEIGHT / 2.0f));
	updateButtonColour(m_gameTypeButtons[static_cast<int>(m_currentButton)], Utilities::MENU_BUTTON_HIGHLIGHTED_COLOUR);
}

void GameTypeScreen::render(SDL_Renderer* t_renderer)
{
	m_renderSystem.render(t_renderer, m_background);
	for (Entity& gameTypeTitle : m_gameTypeTitles)
	{
		m_renderSystem.render(t_renderer, gameTypeTitle);
	}
	for (Entity& gameTypeButton : m_gameTypeButtons)
	{
		m_renderSystem.render(t_renderer, gameTypeButton);
	}
}

void GameTypeScreen::initalise(SDL_Renderer* t_renderer, Controller& t_controller)
{
	setControllerButtonMaps();

	m_background.addComponent(new VisualComponent("Menu_Background.png", t_renderer));
	m_background.addComponent(new TransformComponent(0, 0));

	m_gameTypeButtons[static_cast<int>(GameTypeButtonType::Offline)].addComponent(new VisualComponent("Play_Button.png", t_renderer));
	m_gameTypeButtons[static_cast<int>(GameTypeButtonType::OnlineHost)].addComponent(new VisualComponent("Host_Button.png", t_renderer));
	m_gameTypeButtons[static_cast<int>(GameTypeButtonType::OnlineJoin)].addComponent(new VisualComponent("Join_Button.png", t_renderer));

	m_gameTypeTitles[static_cast<int>(GameTypeTitles::Main)].addComponent(new VisualComponent("Select_Mode_Title.png", t_renderer));
	m_gameTypeTitles[static_cast<int>(GameTypeTitles::Offline)].addComponent(new VisualComponent("Offline_Title.png", t_renderer));
	m_gameTypeTitles[static_cast<int>(GameTypeTitles::Online)].addComponent(new VisualComponent("Online_Title.png", t_renderer));

	float distanceBetweenButtons = Utilities::SCREEN_HEIGHT / 9.0f;

	VisualComponent* visualComp = static_cast<VisualComponent*>(m_gameTypeTitles[static_cast<int>(GameTypeTitles::Main)].getComponent(ComponentType::Visual));
	m_gameTypeTitles[static_cast<int>(GameTypeTitles::Main)].addComponent(new TransformComponent(
		glm::vec2(0.0f, distanceBetweenButtons)
	));

	visualComp = static_cast<VisualComponent*>(m_gameTypeTitles[static_cast<int>(GameTypeTitles::Offline)].getComponent(ComponentType::Visual));
	m_gameTypeTitles[static_cast<int>(GameTypeTitles::Offline)].addComponent(new TransformComponent(
		glm::vec2(0.0f, distanceBetweenButtons * 3.0f)
	));

	visualComp = static_cast<VisualComponent*>(m_gameTypeButtons[static_cast<int>(GameTypeButtonType::Offline)].getComponent(ComponentType::Visual));
	updateButtonColour(m_gameTypeButtons[static_cast<int>(GameTypeButtonType::Offline)], Utilities::MENU_BUTTON_DEFAULT_COLOUR);
	m_gameTypeButtons[static_cast<int>(GameTypeButtonType::Offline)].addComponent(new TransformComponent(
		glm::vec2(0.0f, distanceBetweenButtons * 4.0f)
	));

	visualComp = static_cast<VisualComponent*>(m_gameTypeTitles[static_cast<int>(GameTypeTitles::Online)].getComponent(ComponentType::Visual));
	m_gameTypeTitles[static_cast<int>(GameTypeTitles::Online)].addComponent(new TransformComponent(
		glm::vec2(0.0f, distanceBetweenButtons * 6.0f)
	));

	visualComp = static_cast<VisualComponent*>(m_gameTypeButtons[static_cast<int>(GameTypeButtonType::OnlineHost)].getComponent(ComponentType::Visual));
	updateButtonColour(m_gameTypeButtons[static_cast<int>(GameTypeButtonType::OnlineHost)], Utilities::MENU_BUTTON_DEFAULT_COLOUR);
	m_gameTypeButtons[static_cast<int>(GameTypeButtonType::OnlineHost)].addComponent(new TransformComponent(
		glm::vec2(0.0f, distanceBetweenButtons * 7.0f)
	));

	visualComp = static_cast<VisualComponent*>(m_gameTypeButtons[static_cast<int>(GameTypeButtonType::OnlineJoin)].getComponent(ComponentType::Visual));
	updateButtonColour(m_gameTypeButtons[static_cast<int>(GameTypeButtonType::OnlineJoin)], Utilities::MENU_BUTTON_DEFAULT_COLOUR);
	m_gameTypeButtons[static_cast<int>(GameTypeButtonType::OnlineJoin)].addComponent(new TransformComponent(
		glm::vec2(0.0f, distanceBetweenButtons * 8.0f)
	));

	m_inputEntity.addComponent(new CommandComponent());
	m_inputEntity.addComponent(new InputComponent(t_controller,
		m_controllerButtonMaps[static_cast<int>(ButtonState::Pressed)],
		m_controllerButtonMaps[static_cast<int>(ButtonState::Held)],
		m_controllerButtonMaps[static_cast<int>(ButtonState::Released)]));



	m_renderSystem.setFocus(glm::vec2(Utilities::SCREEN_WIDTH / 2.0f, Utilities::SCREEN_HEIGHT / 2.0f));

	m_eventManager.subscribeToEvent<GameTypeMoveButtonsUpDown>(std::bind(&GameTypeScreen::changeCurrentSelectedButton, this, std::placeholders::_1));
	m_eventManager.subscribeToEvent<GameTypeSelectButton>(std::bind(&GameTypeScreen::buttonPressed, this, std::placeholders::_1));
}

void GameTypeScreen::setControllerButtonMaps()
{
	using ButtonCommandPair = std::pair < ButtonType, Command*>;
	m_controllerButtonMaps[static_cast<int>(ButtonState::Pressed)] =
	{
		/*ButtonCommandPair(ButtonType::DpadUp, new Command()),
		ButtonCommandPair(ButtonType::DpadDown, new Command()),
		ButtonCommandPair(ButtonType::LeftThumbStickUp, new Command()),
		ButtonCommandPair(ButtonType::LeftThumbStickRight, new Command()),
		ButtonCommandPair(ButtonType::A, new Command()),
		ButtonCommandPair(ButtonType::Start, new Command())*/
	};
	m_controllerButtonMaps[static_cast<int>(ButtonState::Held)];
	m_controllerButtonMaps[static_cast<int>(ButtonState::Released)];
}

void GameTypeScreen::createGameTypeButton(Entity& t_gameTypeButton, glm::vec2 t_position)
{
}

void GameTypeScreen::createInputEntity(Controller& t_controller)
{
}

void GameTypeScreen::changeCurrentSelectedButton(const GameTypeMoveButtonsUpDown& t_event)
{
	int currentButtonIndex = static_cast<int>(m_currentButton);
	updateButtonColour(m_gameTypeButtons[currentButtonIndex], Utilities::MENU_BUTTON_DEFAULT_COLOUR);
	currentButtonIndex = t_event.isMoveDown ? currentButtonIndex + 1 : currentButtonIndex - 1;
	currentButtonIndex = glm::clamp(currentButtonIndex, 0, (S_NUMBER_OF_GAME_TYPE_BUTTONS - 1));
	updateButtonColour(m_gameTypeButtons[currentButtonIndex], Utilities::MENU_BUTTON_HIGHLIGHTED_COLOUR);
	m_currentButton = static_cast<GameTypeButtonType>(currentButtonIndex);
}

void GameTypeScreen::updateButtonColour(Entity& t_gameTypeButton, glm::vec3 t_colour)
{
}

void GameTypeScreen::buttonPressed(const GameTypeSelectButton& t_event)
{
	switch (m_currentButton)
	{
	case GameTypeButtonType::Offline:
		m_eventManager.emitEvent(ChangeScreen{ MenuStates::Game });
		break;
	case GameTypeButtonType::OnlineHost:
		// load server and popup ip address
		break;
	case GameTypeButtonType::OnlineJoin:
		// bring up thing to input ip address
		break;
	default:
		break;
	}
}
