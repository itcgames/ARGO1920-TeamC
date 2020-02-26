#include "stdafx.h"
#include "JoinGameScreen.h"

JoinGameScreen::JoinGameScreen(EventManager& t_eventManager, CommandSystem& t_commandSystem, InputSystem& t_inputSystem, RenderSystem& t_renderSystem) :
	m_eventManager{ t_eventManager },
	m_commandSystem{ t_commandSystem },
	m_inputSystem{ t_inputSystem },
	m_renderSystem{ t_renderSystem }
{
}

JoinGameScreen::~JoinGameScreen()
{
}

void JoinGameScreen::update(float t_deltaTime)
{
	m_inputSystem.update(m_inputEntity);
	m_commandSystem.update(m_inputEntity, m_eventManager);
}

void JoinGameScreen::reset()
{
	m_screenActive = true;
	m_renderSystem.setFocus(glm::vec2(Utilities::SCREEN_WIDTH / 2.0f, Utilities::SCREEN_HEIGHT / 2.0f));
}

void JoinGameScreen::render(SDL_Renderer* t_renderer)
{
	m_renderSystem.render(t_renderer, m_background);
	m_renderSystem.render(t_renderer, m_helpText);
	for (int index = 0; index < Utilities::S_MAX_PLAYERS; index++)
	{
		if (m_playersHaveJoined[index])
		{
			for (int index2 = 0; index2 < S_NUMBER_OF_PLAYER_JOINED_ENTITIES_PARTS; index2++)
			{
				m_renderSystem.render(t_renderer, m_playerJoinedEntity[index][index2]);
			}
		}
		else
		{
			m_renderSystem.render(t_renderer, m_playerJoinedEntity[index][static_cast<int>(PlayerJoinedParts::Border)]);
		}
	}
}

void JoinGameScreen::initialise(SDL_Renderer* t_renderer, Controller& t_controller)
{
	m_screenActive = true;
	m_playersHaveJoined[0] = true;
	for (int index = 1; index < Utilities::S_MAX_PLAYERS; index++)
	{
		m_playersHaveJoined[index] = true;
	}
	setControllerButtonMaps();
	createBackground(t_renderer);
	createInputEntity(t_controller);
	createPlayerJoinedEntities(t_renderer);
	createHelpText(t_renderer);
	m_renderSystem.setFocus(glm::vec2(Utilities::SCREEN_WIDTH / 2.0f, Utilities::SCREEN_HEIGHT / 2.0f));
}

void JoinGameScreen::playerHasJoined()
{
	for (int index = 0; index < Utilities::S_MAX_PLAYERS; index++)
	{
		if (!m_playersHaveJoined[index])
		{
			m_playersHaveJoined[index] = true;
			break;
		}
	}
}

void JoinGameScreen::setControllerButtonMaps()
{
	using ButtonCommandPair = std::pair < ButtonType, Command*>;
	m_controllerButtonMaps[static_cast<int>(ButtonState::Pressed)] =
	{
		ButtonCommandPair(ButtonType::A, new MenuConfirmCommand()),
		ButtonCommandPair(ButtonType::Start, new MenuConfirmCommand()),
		ButtonCommandPair(ButtonType::B, new MenuCancelCommand())
	};
	m_controllerButtonMaps[static_cast<int>(ButtonState::Held)];
	m_controllerButtonMaps[static_cast<int>(ButtonState::Released)];
}

void JoinGameScreen::buttonPressed(const MenuButtonPressed& t_event)
{
	if (m_screenActive)
	{

		if (ButtonType::A == t_event.buttonPressed || ButtonType::Start == t_event.buttonPressed)
		{
			startGame();
		}
		else if (ButtonType::B == t_event.buttonPressed)
		{
			cancel();
		}
	}
}

void JoinGameScreen::startGame()
{
	m_screenActive = false;
	m_eventManager.emitEvent<ChangeScreen>(ChangeScreen{ MenuStates::Game });
}

void JoinGameScreen::cancel()
{
	m_screenActive = false;
	m_eventManager.emitEvent<ChangeScreen>(ChangeScreen{ MenuStates::GameType });
}

void JoinGameScreen::createInputEntity(Controller& t_controller)
{
	m_inputEntity.addComponent(new CommandComponent());
	m_inputEntity.addComponent(new InputComponent(t_controller,
		m_controllerButtonMaps[static_cast<int>(ButtonState::Pressed)],
		m_controllerButtonMaps[static_cast<int>(ButtonState::Held)],
		m_controllerButtonMaps[static_cast<int>(ButtonState::Released)]));
}

void JoinGameScreen::createBackground(SDL_Renderer* t_renderer)
{
	m_background.addComponent(new VisualComponent("Menu_Background.png", t_renderer));
	m_background.addComponent(new TransformComponent(0, 0));
}

void JoinGameScreen::createPlayerJoinedEntities(SDL_Renderer* t_renderer)
{
	for (int index = 0; index < Utilities::S_MAX_PLAYERS; index++)
	{
		m_playerJoinedEntity[index][static_cast<int>(PlayerJoinedParts::Border)].addComponent(new VisualComponent("JoinGame_Border.png", t_renderer));
		VisualComponent* borderVisualComp = static_cast<VisualComponent*>(m_playerJoinedEntity[index][static_cast<int>(PlayerJoinedParts::Border)].getComponent(ComponentType::Visual));
		glm::vec2 borderSize = glm::vec2(borderVisualComp->getWidth(), borderVisualComp->getHeight());

		m_playerJoinedEntity[index][static_cast<int>(PlayerJoinedParts::Border)].addComponent(new TransformComponent(glm::vec2(Utilities::SCREEN_WIDTH / 5.0f * (index + 1) - borderSize.x / 2.0f, Utilities::SCREEN_HEIGHT / 3.0f - borderSize.y / 2.0f)));

		m_playerJoinedEntity[index][static_cast<int>(PlayerJoinedParts::Player)].addComponent(new VisualComponent("JoinGame_Player" + std::to_string(index + 1) + ".png", t_renderer));
		TransformComponent* transformComp = static_cast<TransformComponent*>(m_playerJoinedEntity[index][static_cast<int>(PlayerJoinedParts::Border)].getComponent(ComponentType::Transform));
		glm::vec2 borderPosition = transformComp->getPos();
		VisualComponent* playerVisualComp = static_cast<VisualComponent*>(m_playerJoinedEntity[index][static_cast<int>(PlayerJoinedParts::Player)].getComponent(ComponentType::Visual));
		glm::vec2 playerSize = glm::vec2(playerVisualComp->getWidth(), playerVisualComp->getHeight());

		m_playerJoinedEntity[index][static_cast<int>(PlayerJoinedParts::Player)].addComponent(new TransformComponent(glm::vec2(borderPosition.x + borderSize.x / 2.0f - playerSize.x / 2.0f, borderPosition.y + borderSize.y / 2.0f - playerSize.y / 1.5f)));


		TransformComponent* playerTransformComp = static_cast<TransformComponent*>(m_playerJoinedEntity[index][static_cast<int>(PlayerJoinedParts::Player)].getComponent(ComponentType::Transform));

		m_playerJoinedEntity[index][static_cast<int>(PlayerJoinedParts::Text)].addComponent(new TextComponent("ariblk.ttf", t_renderer, 20, true, std::string("Player " + std::to_string(index + 1) + " Joined" ), Utilities::UI_COLOUR.x, Utilities::UI_COLOUR.y, Utilities::UI_COLOUR.z));
		TextComponent* textComp = static_cast<TextComponent*>(m_playerJoinedEntity[index][static_cast<int>(PlayerJoinedParts::Text)].getComponent(ComponentType::Text));
		glm::vec2 textSize = glm::vec2(textComp->getWidth(), textComp->getHeight());

		m_playerJoinedEntity[index][static_cast<int>(PlayerJoinedParts::Text)].addComponent(new TransformComponent(glm::vec2(playerTransformComp->getPos().x + playerSize.x / 2.0f - textSize.x / 2.0f, playerSize.y + playerTransformComp->getPos().y + textSize.y)));
	}
}

void JoinGameScreen::createHelpText(SDL_Renderer* t_renderer)
{
	m_helpText.addComponent(new TextComponent("ariblk.ttf", t_renderer, Utilities::MEDIUM_FONT, true, std::string("Press Start or A To Begin Game! Press B To Return"), 255, 255, 255));
	TextComponent* textComp = static_cast<TextComponent*>(m_helpText.getComponent(ComponentType::Text));
	glm::vec2 textSize = glm::vec2(textComp->getWidth(), textComp->getHeight());
	m_helpText.addComponent(new TransformComponent(glm::vec2(Utilities::SCREEN_WIDTH / 2.0f - textSize.x / 2.0f, Utilities::SCREEN_HEIGHT * (5.0f / 6.0f))));
}
