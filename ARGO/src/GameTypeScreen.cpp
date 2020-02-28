#include "stdafx.h"
#include "GameTypeScreen.h"

GameTypeScreen::GameTypeScreen(EventManager& t_eventManager, CommandSystem& t_commandSystem, InputSystem& t_inputSystem, RenderSystem& t_renderSystem, OnlineGameHandler& t_onlineHandler) :
	m_eventManager{ t_eventManager },
	m_commandSystem{ t_commandSystem },
	m_inputSystem{ t_inputSystem },
	m_renderSystem{ t_renderSystem },
	m_onlineHandler{ t_onlineHandler }
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
	m_currentButton = MenuButtonsType::Offline;
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

	if (m_joinPopupActive)
	{
		m_renderSystem.render(t_renderer, m_popup);
		for (Entity& joinTextLine : m_joinText)
		{
			m_renderSystem.render(t_renderer, joinTextLine);
		}
		for (int index = 0; index < S_IP_NUMBER_LENGTH; index++)
		{
			m_renderSystem.render(t_renderer, m_ipNumbers[index][static_cast<int>(DialEntityType::Dial)]);
			m_renderSystem.render(t_renderer, m_ipNumbers[index][static_cast<int>(DialEntityType::Text)]);
			if (index == m_currentSelectedIpNumber)
			{
				m_renderSystem.render(t_renderer, m_ipNumbers[index][static_cast<int>(DialEntityType::Arrows)]);
			}
		}
		for (Entity& dot : m_ipDots)
		{
			m_renderSystem.render(t_renderer, dot);
		}
	}
}

void GameTypeScreen::initialise(SDL_Renderer* t_renderer, Controller& t_controller)
{
	m_screenActive = true;
	setControllerButtonMaps();

	createBackgroundEntity(t_renderer);
	createMenuButtonsAndTitles(t_renderer);
	createInputEntity(t_controller);
	createPopupEntity(t_renderer);
	glm::vec2 popUpPos = static_cast<TransformComponent*>(m_popup.getComponent(ComponentType::Transform))->getPos();
	float popUpHeight = static_cast<VisualComponent*>(m_popup.getComponent(ComponentType::Visual))->getHeight();
	createJoinText(t_renderer, popUpPos, popUpHeight);
	createIpInputter(t_renderer, popUpPos);

	m_renderSystem.setFocus(glm::vec2(Utilities::SCREEN_WIDTH / 2.0f, Utilities::SCREEN_HEIGHT / 2.0f));

	m_eventManager.subscribeToEvent<Events::MenuMoveBetweenUI>(std::bind(&GameTypeScreen::moveThroughUI, this, std::placeholders::_1));
	m_eventManager.subscribeToEvent<Events::MenuButtonPressed>(std::bind(&GameTypeScreen::buttonPressed, this, std::placeholders::_1));
}

void GameTypeScreen::setIsActive(bool t_isActive)
{
	m_screenActive = t_isActive;
}

bool GameTypeScreen::getIsActive()
{
	return m_screenActive;
}

void GameTypeScreen::attemptToConnect(std::string t_ip, int t_port)
{
	m_onlineHandler.connectToServer(t_ip, t_port);
}

void GameTypeScreen::setControllerButtonMaps()
{
	using ButtonCommandPair = std::pair < ButtonType, Command*>;
	m_controllerButtonMaps[static_cast<int>(ButtonState::Pressed)] =
	{
		ButtonCommandPair(ButtonType::DpadUp, new MenuMoveUpCommand()),
		ButtonCommandPair(ButtonType::DpadDown, new MenuMoveDownCommand()),
		ButtonCommandPair(ButtonType::DpadLeft, new MenuMoveLeftCommand()),
		ButtonCommandPair(ButtonType::DpadRight, new MenuMoveRightCommand()),

		ButtonCommandPair(ButtonType::A, new MenuSelectButtonCommand()),
		ButtonCommandPair(ButtonType::Start, new MenuConfirmCommand()),
		ButtonCommandPair(ButtonType::B, new MenuCancelCommand())
	};
	m_controllerButtonMaps[static_cast<int>(ButtonState::Held)];
	m_controllerButtonMaps[static_cast<int>(ButtonState::Released)];
}


void GameTypeScreen::createBackgroundEntity(SDL_Renderer* t_renderer)
{
	m_background.addComponent(new VisualComponent("Menu_Background.png", t_renderer));
	m_background.addComponent(new TransformComponent(0, 0));
}

void GameTypeScreen::createMenuButtonsAndTitles(SDL_Renderer* t_renderer)
{
	m_gameTypeButtons[static_cast<int>(MenuButtonsType::Offline)].addComponent(new VisualComponent("Play_Button.png", t_renderer));
	m_gameTypeButtons[static_cast<int>(MenuButtonsType::OnlineHost)].addComponent(new VisualComponent("Host_Button.png", t_renderer));
	m_gameTypeButtons[static_cast<int>(MenuButtonsType::OnlineJoin)].addComponent(new VisualComponent("Join_Button.png", t_renderer));

	m_gameTypeTitles[static_cast<int>(MenuTitles::Main)].addComponent(new VisualComponent("Select_Mode_Title.png", t_renderer));
	m_gameTypeTitles[static_cast<int>(MenuTitles::Offline)].addComponent(new VisualComponent("Offline_Title.png", t_renderer));
	m_gameTypeTitles[static_cast<int>(MenuTitles::Online)].addComponent(new VisualComponent("Online_Title.png", t_renderer));

	float distanceBetweenButtons = Utilities::SCREEN_HEIGHT / 9.0f;

	VisualComponent* visualComp = static_cast<VisualComponent*>(m_gameTypeTitles[static_cast<int>(MenuTitles::Main)].getComponent(ComponentType::Visual));
	glm::vec2 entitySize = glm::vec2(visualComp->getWidth(), visualComp->getHeight());
	m_gameTypeTitles[static_cast<int>(MenuTitles::Main)].addComponent(new TransformComponent(
		glm::vec2(Utilities::SCREEN_WIDTH / 2.0f - entitySize.x / 2.0f, distanceBetweenButtons)
	));

	visualComp = static_cast<VisualComponent*>(m_gameTypeTitles[static_cast<int>(MenuTitles::Offline)].getComponent(ComponentType::Visual));
	entitySize = glm::vec2(visualComp->getWidth(), visualComp->getHeight());
	m_gameTypeTitles[static_cast<int>(MenuTitles::Offline)].addComponent(new TransformComponent(
		glm::vec2(Utilities::SCREEN_WIDTH / 2.0f - entitySize.x / 2.0f, distanceBetweenButtons * 3.0f)
	));

	visualComp = static_cast<VisualComponent*>(m_gameTypeButtons[static_cast<int>(MenuButtonsType::Offline)].getComponent(ComponentType::Visual));
	entitySize = glm::vec2(visualComp->getWidth(), visualComp->getHeight());
	updateButtonColour(m_gameTypeButtons[static_cast<int>(MenuButtonsType::Offline)], Utilities::MENU_BUTTON_DEFAULT_COLOUR);
	m_gameTypeButtons[static_cast<int>(MenuButtonsType::Offline)].addComponent(new TransformComponent(
		glm::vec2(Utilities::SCREEN_WIDTH / 2.0f - entitySize.x / 2.0f, distanceBetweenButtons * 4.0f)
	));

	visualComp = static_cast<VisualComponent*>(m_gameTypeTitles[static_cast<int>(MenuTitles::Online)].getComponent(ComponentType::Visual));
	entitySize = glm::vec2(visualComp->getWidth(), visualComp->getHeight());
	m_gameTypeTitles[static_cast<int>(MenuTitles::Online)].addComponent(new TransformComponent(
		glm::vec2(Utilities::SCREEN_WIDTH / 2.0f - entitySize.x / 2.0f, distanceBetweenButtons * 6.0f)
	));

	visualComp = static_cast<VisualComponent*>(m_gameTypeButtons[static_cast<int>(MenuButtonsType::OnlineHost)].getComponent(ComponentType::Visual));
	entitySize = glm::vec2(visualComp->getWidth(), visualComp->getHeight());
	updateButtonColour(m_gameTypeButtons[static_cast<int>(MenuButtonsType::OnlineHost)], Utilities::MENU_BUTTON_DEFAULT_COLOUR);
	m_gameTypeButtons[static_cast<int>(MenuButtonsType::OnlineHost)].addComponent(new TransformComponent(
		glm::vec2(Utilities::SCREEN_WIDTH / 2.0f - entitySize.x / 2.0f, distanceBetweenButtons * 7.0f)
	));

	visualComp = static_cast<VisualComponent*>(m_gameTypeButtons[static_cast<int>(MenuButtonsType::OnlineJoin)].getComponent(ComponentType::Visual));
	entitySize = glm::vec2(visualComp->getWidth(), visualComp->getHeight());
	updateButtonColour(m_gameTypeButtons[static_cast<int>(MenuButtonsType::OnlineJoin)], Utilities::MENU_BUTTON_DEFAULT_COLOUR);
	m_gameTypeButtons[static_cast<int>(MenuButtonsType::OnlineJoin)].addComponent(new TransformComponent(
		glm::vec2(Utilities::SCREEN_WIDTH / 2.0f - entitySize.x / 2.0f, distanceBetweenButtons * 8.0f)
	));

	updateButtonColour(m_gameTypeButtons[static_cast<int>(m_currentButton)], Utilities::MENU_BUTTON_HIGHLIGHTED_COLOUR);
}

void GameTypeScreen::createPopupEntity(SDL_Renderer* t_renderer)
{
	m_popup.addComponent(new VisualComponent("Pop_Up.png", t_renderer));
	VisualComponent* visualComp = static_cast<VisualComponent*>(m_popup.getComponent(ComponentType::Visual));
	m_popup.addComponent(new TransformComponent(glm::vec2(Utilities::SCREEN_WIDTH / 2.0f - visualComp->getWidth() / 2.0f, Utilities::SCREEN_HEIGHT / 2.0f - visualComp->getHeight() / 2.0f)));
}

void GameTypeScreen::createInputEntity(Controller& t_controller)
{
	m_inputEntity.addComponent(new CommandComponent());
	m_inputEntity.addComponent(new InputComponent(t_controller,
		m_controllerButtonMaps[static_cast<int>(ButtonState::Pressed)],
		m_controllerButtonMaps[static_cast<int>(ButtonState::Held)],
		m_controllerButtonMaps[static_cast<int>(ButtonState::Released)]));
}

void GameTypeScreen::createJoinText(SDL_Renderer* t_renderer, glm::vec2 t_popUpPos, float t_popUpHeight)
{

	std::string linesText[S_NUMBER_OF_JOIN_TEXT_LINES] =
	{
		"Enter Host's IP",
		"Press Start To Join"
	};
	for (int index = 0; index < S_NUMBER_OF_JOIN_TEXT_LINES; index++)
	{
		m_joinText[index].addComponent(new TextComponent("ariblk.ttf", t_renderer, Utilities::MEDIUM_FONT, true, linesText[index], Utilities::UI_COLOUR.x, Utilities::UI_COLOUR.y, Utilities::UI_COLOUR.z));
		TextComponent* textComponent = static_cast<TextComponent*>(m_joinText[index].getComponent(ComponentType::Text));
		m_joinText[index].addComponent(new TransformComponent(glm::vec2(Utilities::SCREEN_WIDTH / 2.0f - textComponent->getWidth() / 2.0f, textComponent->getHeight() / 2.0f + t_popUpPos.y + (t_popUpHeight / 6.0f) * index)));
	}
}

void GameTypeScreen::createIpInputter(SDL_Renderer* t_renderer, glm::vec2 t_popupPos)
{
	for (int index = 0; index < S_IP_NUMBER_LENGTH; index++)
	{
		m_ipNumbers[index][static_cast<int>(DialEntityType::Dial)].addComponent(new VisualComponent("Dial.png", t_renderer));
		VisualComponent* visualComp = static_cast<VisualComponent*>(m_ipNumbers[index][static_cast<int>(DialEntityType::Dial)].getComponent(ComponentType::Visual));
		glm::vec2 dialSize = glm::vec2(visualComp->getWidth(), visualComp->getHeight());

		m_ipNumbers[index][static_cast<int>(DialEntityType::Dial)].addComponent(new TransformComponent(glm::vec2(t_popupPos.x + dialSize.x + Utilities::SCREEN_WIDTH / 14.0f * (index), Utilities::SCREEN_HEIGHT / 2.0f)));
		TransformComponent* transformComp = static_cast<TransformComponent*>(m_ipNumbers[index][static_cast<int>(DialEntityType::Dial)].getComponent(ComponentType::Transform));

		m_ipNumbers[index][static_cast<int>(DialEntityType::Arrows)].addComponent(new VisualComponent("Dial_Arrows.png", t_renderer));
		m_ipNumbers[index][static_cast<int>(DialEntityType::Arrows)].addComponent(new TransformComponent(transformComp->getPos()));

		m_ipNumbers[index][static_cast<int>(DialEntityType::Text)].addComponent(new TextComponent("ariblk.ttf", t_renderer, Utilities::MEDIUM_FONT, true, std::to_string(ipNumberValues[index]), Utilities::UI_COLOUR.x, Utilities::UI_COLOUR.y, Utilities::UI_COLOUR.z));
		TextComponent* textComp = static_cast<TextComponent*>(m_ipNumbers[index][static_cast<int>(DialEntityType::Text)].getComponent(ComponentType::Text));
		glm::vec2 textSize = glm::vec2(textComp->getWidth(), textComp->getHeight());
		m_ipNumbers[index][static_cast<int>(DialEntityType::Text)].addComponent(new TransformComponent(transformComp->getPos() + glm::vec2(dialSize.x / 2.0f - textSize.x / 2.0f, dialSize.y / 2.0f - textSize.y / 2.0f)));

		if ((index + 1) % 3 == 0 && (index + 1) != S_IP_NUMBER_LENGTH)
		{
			int dotIndex = ((index + 1) / 3) - 1;

			m_ipDots[dotIndex].addComponent(new VisualComponent("Dial_Selector.png", t_renderer));
			VisualComponent* dotVisualComp = static_cast<VisualComponent*>(m_ipDots[dotIndex].getComponent(ComponentType::Visual));
			glm::vec2 dotSize = glm::vec2(dotVisualComp->getWidth(), dotVisualComp->getHeight());

			glm::vec2 dialOnePos = static_cast<TransformComponent*>(m_ipNumbers[index - 1][static_cast<int>(DialEntityType::Dial)].getComponent(ComponentType::Transform))->getPos();
			glm::vec2 dialTwoPos = static_cast<TransformComponent*>(m_ipNumbers[index][static_cast<int>(DialEntityType::Dial)].getComponent(ComponentType::Transform))->getPos();

			float dotPosX = ((dialOnePos.x + dialTwoPos.x) / 2.0f) + (dialSize.x / 2.0f) - (dotSize.x / 2.0f) + glm::distance(dialOnePos.x, dialTwoPos.x);
			float dotPosY = dialOnePos.y + (dialSize.y / 2.0f) - (dotSize.y / 2.0f);

			m_ipDots[dotIndex].addComponent(new TransformComponent(glm::vec2(dotPosX, dotPosY)));
		}
	}
}

void GameTypeScreen::moveThroughUI(const Events::MenuMoveBetweenUI& t_event)
{
	if (m_screenActive)
	{
		if (!m_joinPopupActive)
		{
			updateCurrentButton(t_event.direction);
		}
		else
		{
			updateIpDial(t_event.direction);
		}
	}
}

void GameTypeScreen::updateButtonColour(Entity& t_gameTypeButton, glm::vec3 t_colour)
{
	VisualComponent* visualComp = static_cast<VisualComponent*>(t_gameTypeButton.getComponent(ComponentType::Visual));
	visualComp->setColor(t_colour.x, t_colour.y, t_colour.z);
}

void GameTypeScreen::buttonPressed(const Events::MenuButtonPressed& t_event)
{
	if (m_screenActive)
	{
		if (ButtonType::A == t_event.buttonPressed)
		{
			gameTypeChosen();
		}
		else if (ButtonType::B == t_event.buttonPressed)
		{
			gameTypeCancel();
		}
		else if (ButtonType::Start == t_event.buttonPressed)
		{
			gameTypeConfirmed();
		}
	}
}

void GameTypeScreen::gameTypeConfirmed()
{
	if (m_joinPopupActive)
	{
		std::string ipValue;
		for (int index = 0; index < S_IP_NUMBER_LENGTH; index++)
		{
			if (index == 0 || index % 3 == 0)
			{
				if (ipNumberValues[index] == 0)
				{
					if (ipNumberValues[index + 1] == 0)
					{
						index += 2;
					}
					else
					{
						index++;
					}
				}
			}
			ipValue += std::to_string(ipNumberValues[index]);
			if ((index + 1) % 3 == 0 && (index + 1 != S_IP_NUMBER_LENGTH))
			{
				ipValue += ".";
			}
		}

#ifdef _DEBUG
		if (ipValue == "0.0.0.0")
		{
			findHostsIp();
			ipValue = m_hostsIp;
		}
		std::cout << ipValue << std::endl;
#endif // _DEBUG

		//join server
		attemptToConnect(ipValue);

		//switch screen/state to lobby screen with players

		m_screenActive = false;
		m_eventManager.emitEvent<Events::ChangeScreen>(Events::ChangeScreen{ MenuStates::JoinGame });
 	} 
} 

void GameTypeScreen::gameTypeCancel()
{
	Utilities::OnlineData::S_ONLINE_STATUS = Utilities::OnlineState::Local;
	Utilities::OnlineData::S_IS_HOST = false;

	if (m_joinPopupActive)
	{
		m_joinPopupActive = false;
	} 
	else
	{
		m_screenActive = false;
		m_eventManager.emitEvent<Events::ChangeScreen>(Events::ChangeScreen{ MenuStates::MainMenu });
	}
}

void GameTypeScreen::gameTypeChosen()
{
	switch (m_currentButton)
	{
	case MenuButtonsType::Offline:
		m_screenActive = false;
		m_eventManager.emitEvent(Events::ChangeScreen{ MenuStates::JoinGame });
		break;
	case MenuButtonsType::OnlineHost:
	{
		//set up process info variables
		ZeroMemory(&m_startupInfo, sizeof(m_startupInfo));
		m_startupInfo.cb = sizeof(m_startupInfo);
		ZeroMemory(&m_processInfo, sizeof(m_processInfo));

		std::string path = SDL_GetBasePath();
		path.append("ScuffedArgoServer.exe");
		LPCSTR name = path.c_str();

		// Start the child process. 
		if (!CreateProcess(
			name,	// No module name (use command line)
			NULL,			// Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&m_startupInfo,	// Pointer to STARTUPINFO structure
			&m_processInfo)	// Pointer to PROCESS_INFORMATION structure
			)
		{
			printf("Failed to start the server!\nError:(%d).\n", GetLastError());
		}
		else
		{
			Utilities::OnlineData::S_ONLINE_STATUS = Utilities::OnlineState::Host;
			Utilities::OnlineData::S_IS_HOST = true;
			attemptToConnect(m_hostsIp);
			m_screenActive = false;
			m_eventManager.emitEvent<Events::ChangeScreen>(Events::ChangeScreen{ MenuStates::JoinGame });
		}
		break;
	}
	case MenuButtonsType::OnlineJoin:
		// bring up thing to input ip address
		m_joinPopupActive = true;
		break;
	default:
		break;
	}
}

void GameTypeScreen::updateIpDial(MoveDirection t_inputDirection)
{
	if (MoveDirection::Up == t_inputDirection)
	{
		ipNumberValues[m_currentSelectedIpNumber]++;
		if (ipNumberValues[m_currentSelectedIpNumber] > 9)
		{
			ipNumberValues[m_currentSelectedIpNumber] = 0;
		}
		TextComponent* textComp = static_cast<TextComponent*>(m_ipNumbers[m_currentSelectedIpNumber][1].getComponent(ComponentType::Text));
		textComp->setText(std::to_string(ipNumberValues[m_currentSelectedIpNumber]));
	}
	else if (MoveDirection::Down == t_inputDirection)
	{
		ipNumberValues[m_currentSelectedIpNumber]--;
		if (ipNumberValues[m_currentSelectedIpNumber] < 0)
		{
			ipNumberValues[m_currentSelectedIpNumber] = 9;
		}
		TextComponent* textComp = static_cast<TextComponent*>(m_ipNumbers[m_currentSelectedIpNumber][1].getComponent(ComponentType::Text));
		textComp->setText(std::to_string(ipNumberValues[m_currentSelectedIpNumber]));
	}
	else if (MoveDirection::Right == t_inputDirection)
	{
		m_currentSelectedIpNumber++;
		if (m_currentSelectedIpNumber >= S_IP_NUMBER_LENGTH)
		{
			m_currentSelectedIpNumber = 0;
		}
	}
	else if (MoveDirection::Left == t_inputDirection)
	{
		m_currentSelectedIpNumber--;
		if (m_currentSelectedIpNumber < 0)
		{
			m_currentSelectedIpNumber = S_IP_NUMBER_LENGTH - 1;
		}
	}
}

void GameTypeScreen::updateCurrentButton(MoveDirection t_inputDirection)
{
	int currentButtonIndex = static_cast<int>(m_currentButton);
	updateButtonColour(m_gameTypeButtons[currentButtonIndex], Utilities::MENU_BUTTON_DEFAULT_COLOUR);
	if (MoveDirection::Up == t_inputDirection)
	{
		currentButtonIndex--;
		if (currentButtonIndex < 0)
		{
			currentButtonIndex = static_cast<int>(MenuButtonsType::OnlineJoin);
		}
	}
	else if (MoveDirection::Down == t_inputDirection)
	{
		currentButtonIndex++;
		if (currentButtonIndex > S_NUMBER_OF_GAME_TYPE_BUTTONS - 1)
		{
			currentButtonIndex = static_cast<int>(MenuButtonsType::Offline);
		}
	}
	updateButtonColour(m_gameTypeButtons[currentButtonIndex], Utilities::MENU_BUTTON_HIGHLIGHTED_COLOUR);
	m_currentButton = static_cast<MenuButtonsType>(currentButtonIndex);
}

void GameTypeScreen::findHostsIp()
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