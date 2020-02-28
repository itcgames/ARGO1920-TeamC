#include "stdafx.h"
#include "OptionsScreen.h"

OptionsScreen::OptionsScreen(EventManager& t_eventManager, CommandSystem& t_commandSystem, InputSystem& t_input, RenderSystem& t_renderSystem) :
	m_eventManager{ t_eventManager },
	m_commandSystem{ t_commandSystem },
	m_inputSystem{ t_input },
	m_renderSystem{ t_renderSystem }
{
}

OptionsScreen::~OptionsScreen()
{
}

void OptionsScreen::update(float t_deltaTime)
{
	m_inputSystem.update(m_inputEntity);
	m_commandSystem.update(m_inputEntity, m_eventManager);
}

void OptionsScreen::render(SDL_Renderer* t_renderer)
{
	m_renderSystem.render(t_renderer, m_background);
	m_renderSystem.render(t_renderer, m_helpEntity);

}

void OptionsScreen::reset()
{
	m_renderSystem.setFocus(glm::vec2(Utilities::SCREEN_WIDTH / 2.0f, Utilities::SCREEN_HEIGHT / 2.0f));
}

void OptionsScreen::initialise(SDL_Renderer* t_renderer, Controller& t_controller)
{
	setControllerButtonMaps();

	m_background.addComponent(new VisualComponent("Menu_Background.png", t_renderer));
	m_background.addComponent(new TransformComponent(0, 0));

	m_helpEntity.addComponent(new VisualComponent("Controls.png", t_renderer));
	m_helpEntity.addComponent(new TransformComponent(0, 0));

	m_renderSystem.setFocus(glm::vec2(Utilities::SCREEN_WIDTH / 2.0f, Utilities::SCREEN_HEIGHT / 2.0f));

	m_inputEntity.addComponent(new InputComponent(t_controller,
		m_controllerButtonMaps[static_cast<int>(ButtonState::Pressed)],
		m_controllerButtonMaps[static_cast<int>(ButtonState::Held)],
		m_controllerButtonMaps[static_cast<int>(ButtonState::Released)]));
	m_inputEntity.addComponent(new CommandComponent());
}

void OptionsScreen::setControllerButtonMaps()
{
	using ButtonCommandPair = std::pair<ButtonType, Command*>;
	m_controllerButtonMaps[static_cast<int>(ButtonState::Pressed)] =
	{
		ButtonCommandPair(ButtonType::B, new GoToMainMenuCommand())
	};
	m_controllerButtonMaps[static_cast<int>(ButtonState::Held)];
	m_controllerButtonMaps[static_cast<int>(ButtonState::Released)];
}

