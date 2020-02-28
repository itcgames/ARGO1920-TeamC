#include "stdafx.h"
#include "JoinGameScreen.h"

JoinGameScreen::JoinGameScreen(EventManager& t_eventManager, CommandSystem& t_commandSystem, InputSystem& t_inputSystem, RenderSystem& t_renderSystem, Controller(&t_controllers)[Utilities::S_MAX_PLAYERS]) :
	m_eventManager{ t_eventManager },
	m_commandSystem{ t_commandSystem },
	m_inputSystem{ t_inputSystem },
	m_renderSystem{ t_renderSystem },
	m_controllers{t_controllers}
{ 
}

JoinGameScreen::~JoinGameScreen()
{
}

void JoinGameScreen::update(float t_deltaTime)
{
	for (Entity& input : m_inputEntities)
	{
		m_inputSystem.update(input);
		m_commandSystem.update(input, m_eventManager);
	}
}

void JoinGameScreen::reset()
{
	m_screenActive = true;
	m_renderSystem.setFocus(glm::vec2(Utilities::SCREEN_WIDTH / 2.0f, Utilities::SCREEN_HEIGHT / 2.0f));
}

void JoinGameScreen::render(SDL_Renderer* t_renderer)
{
	m_renderSystem.render(t_renderer, m_background);
	for (Entity& text : m_helpText)
	{
		m_renderSystem.render(t_renderer, text);
	}
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

void JoinGameScreen::initialise(SDL_Renderer* t_renderer)
{
	m_screenActive = true;
	m_playersHaveJoined[0] = true;
	for (int index = 1; index < Utilities::S_MAX_PLAYERS; index++)
	{
		m_playersHaveJoined[index] = true;
		m_joinedControllers[index] = Controller();
	}
	m_joinedControllers[0] = m_controllers[0];
	setControllerButtonMaps();
	createBackground(t_renderer);
	createInputEntities();
	createPlayerJoinedEntities(t_renderer);
	createHelpText(t_renderer);
	m_renderSystem.setFocus(glm::vec2(Utilities::SCREEN_WIDTH / 2.0f, Utilities::SCREEN_HEIGHT / 2.0f));
	m_eventManager.subscribeToEvent<Events::MenuButtonPressed>(std::bind(&JoinGameScreen::buttonPressed, this, std::placeholders::_1));
	m_eventManager.subscribeToEvent<Events::JoinGame>(std::bind(&JoinGameScreen::playersJoin, this, std::placeholders::_1));

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
	m_controllerButtonMaps[0] =
	{
		ButtonCommandPair(ButtonType::A, new MenuConfirmCommand()),
		ButtonCommandPair(ButtonType::Start, new MenuConfirmCommand()),
		ButtonCommandPair(ButtonType::B, new MenuCancelCommand())
	};
	m_controllerButtonMaps[1] =
	{
		ButtonCommandPair(ButtonType::A, new PlayerTwoJoinCommand()) 
	}; 
	m_controllerButtonMaps[2] =
	{
		ButtonCommandPair(ButtonType::A, new PlayerThreeJoinCommand())
	}; 
	m_controllerButtonMaps[3] =
	{
		ButtonCommandPair(ButtonType::A, new PlayerFourJoinCommand())
	};
}

void JoinGameScreen::buttonPressed(const Events::MenuButtonPressed& t_event)
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
	m_eventManager.emitEvent<Events::SetControllers>(Events::SetControllers{ m_joinedControllers });
	m_eventManager.emitEvent<Events::ChangeScreen>(Events::ChangeScreen{ MenuStates::Game });
}

void JoinGameScreen::cancel()
{
	system("taskkill /F /T /IM ScuffedArgoServer.exe");
	m_screenActive = false;
	m_eventManager.emitEvent<Events::ChangeScreen>(Events::ChangeScreen{ MenuStates::GameType });
}

void JoinGameScreen::createInputEntities()
{
	for (int index = 0; index < Utilities::S_MAX_PLAYERS; index++)
	{
		m_inputEntities[index].addComponent(new CommandComponent());
		m_inputEntities[index].addComponent(new InputComponent(m_controllers[index],
			m_controllerButtonMaps[static_cast<int>(ButtonState::Pressed)],
			m_controllerButtonMaps[static_cast<int>(ButtonState::Held)],
			m_controllerButtonMaps[static_cast<int>(ButtonState::Released)]));
	}
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

		m_playerJoinedEntity[index][static_cast<int>(PlayerJoinedParts::Text)].addComponent(new TextComponent("ariblk.ttf", t_renderer, 20, true, std::string("Player " + std::to_string(index + 1) + " Joined"), Utilities::UI_COLOUR.x, Utilities::UI_COLOUR.y, Utilities::UI_COLOUR.z));
		TextComponent* textComp = static_cast<TextComponent*>(m_playerJoinedEntity[index][static_cast<int>(PlayerJoinedParts::Text)].getComponent(ComponentType::Text));
		glm::vec2 textSize = glm::vec2(textComp->getWidth(), textComp->getHeight());

		m_playerJoinedEntity[index][static_cast<int>(PlayerJoinedParts::Text)].addComponent(new TransformComponent(glm::vec2(playerTransformComp->getPos().x + playerSize.x / 2.0f - textSize.x / 2.0f, playerSize.y + playerTransformComp->getPos().y + textSize.y)));
	}
}

void JoinGameScreen::createHelpText(SDL_Renderer* t_renderer)
{
	findHostsIp();

	m_helpText[0].addComponent(new TextComponent("ariblk.ttf", t_renderer, Utilities::MEDIUM_FONT, true, m_hostsIp, 255, 255, 255));
	TextComponent* textComp = static_cast<TextComponent*>(m_helpText[0].getComponent(ComponentType::Text));
	glm::vec2 textSize = glm::vec2(textComp->getWidth(), textComp->getHeight());
	m_helpText[0].addComponent(new TransformComponent(glm::vec2(Utilities::SCREEN_WIDTH / 2.0f - textSize.x / 2.0f, Utilities::SCREEN_HEIGHT * (4.0f / 6.0f))));

	m_helpText[1].addComponent(new TextComponent("ariblk.ttf", t_renderer, Utilities::MEDIUM_FONT, true, std::string("Press Start or A To Begin Game! Press B To Return"), 255, 255, 255));
	textComp = static_cast<TextComponent*>(m_helpText[1].getComponent(ComponentType::Text));
	textSize = glm::vec2(textComp->getWidth(), textComp->getHeight());
	m_helpText[1].addComponent(new TransformComponent(glm::vec2(Utilities::SCREEN_WIDTH / 2.0f - textSize.x / 2.0f, Utilities::SCREEN_HEIGHT * (5.0f / 6.0f))));
}

void JoinGameScreen::findHostsIp()
{
	system("ipconfig > ipinfo.txt");
	std::ifstream ipFile;
	ipFile.open("ipinfo.txt");
	std::string lineContents;
	std::string ipValue;
	if (ipFile.is_open())
	{
		while (std::getline(ipFile, lineContents))
		{
			if (lineContents.find("IPv4") != std::string::npos)
			{
				int here = 0;
				ipValue = lineContents.substr(lineContents.find(": ") + 2);
				break;
			}
		}
	}
	m_hostsIp = ipValue;
}

void JoinGameScreen::playersJoin(const Events::JoinGame& t_event)
{
	if (t_event.index == 1)
	{
		m_joinedControllers[1] = m_controllers[1];
	}
	else if (t_event.index == 2)
	{
		m_joinedControllers[2] = m_controllers[2];

	}
	else if (t_event.index == 3)
	{
		m_joinedControllers[3] = m_controllers[3];

	}
}