#include "stdafx.h"
#include "LicenseScreen.h"

LicenseScreen::LicenseScreen(EventManager& t_eventManager, CommandSystem& t_commandSystem, InputSystem& t_input, RenderSystem& t_renderSystem) :
	m_eventManager{ t_eventManager },
	m_commandSystem{ t_commandSystem },
	m_inputSystem{ t_input },
	m_renderSystem{ t_renderSystem }
{
}

LicenseScreen::~LicenseScreen()
{
}

void LicenseScreen::update(float t_deltaTime)
{
	m_inputSystem.update(m_inputEntity);
	m_commandSystem.update(m_inputEntity, m_eventManager);
	if (SDL_GetTicks() - m_screenStartTime > S_TIME_ON_SCREEN)
	{
		m_eventManager.emitEvent(ChangeScreen{ MenuStates::MainMenu });
	}
}

void LicenseScreen::render(SDL_Renderer* t_renderer)
{
	m_renderSystem.render(t_renderer, m_background);
}

void LicenseScreen::reset()
{
	m_screenActive = true;
	m_renderSystem.setFocus(glm::vec2(Utilities::SCREEN_WIDTH / 2.0f, Utilities::SCREEN_HEIGHT / 2.0f));
	m_screenStartTime = SDL_GetTicks();
}

void LicenseScreen::initialise(SDL_Renderer* t_renderer, Controller& t_controller)
{
	m_screenActive = true;
	setControllerButtonMaps();

	m_background.addComponent(new VisualComponent("License_Background.png", t_renderer));
	m_background.addComponent(new TransformComponent(0, 0));

	m_renderSystem.setFocus(glm::vec2(Utilities::SCREEN_WIDTH / 2.0f, Utilities::SCREEN_HEIGHT / 2.0f));

	m_inputEntity.addComponent(new InputComponent(t_controller,
		m_controllerButtonMaps[static_cast<int>(ButtonState::Pressed)],
		m_controllerButtonMaps[static_cast<int>(ButtonState::Held)],
		m_controllerButtonMaps[static_cast<int>(ButtonState::Released)]));
	m_inputEntity.addComponent(new CommandComponent());

	m_screenStartTime = SDL_GetTicks();
} 

void LicenseScreen::setControllerButtonMaps()
{
	using ButtonCommandPair = std::pair<ButtonType, Command*>;
	m_controllerButtonMaps[static_cast<int>(ButtonState::Pressed)] =
	{
		ButtonCommandPair(ButtonType::A, new MenuSelectButtonCommand()),
		ButtonCommandPair(ButtonType::Start, new MenuConfirmCommand())
	};
	m_controllerButtonMaps[static_cast<int>(ButtonState::Held)];
	m_controllerButtonMaps[static_cast<int>(ButtonState::Released)];
}
