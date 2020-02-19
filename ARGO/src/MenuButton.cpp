#include "stdafx.h"
#include "MenuButton.h"
#include "..\include\MenuButton.h"

MenuButton::MenuButton(glm::vec2 t_position, glm::vec2 t_size, SDL_Texture* t_texture) 
{
	init(t_position, t_size, t_texture);
}

MenuButton::~MenuButton()
{
}

void MenuButton::init(glm::vec2 t_position, glm::vec2 t_size, SDL_Texture* t_texture)
{

	m_position = t_position;
	m_size = t_size;
	m_buttonTexture = t_texture;
}

void MenuButton::setPosition(glm::vec2 t_position)
{
	m_position = t_position;
}

glm::vec2 MenuButton::getPosition()
{
	return m_position;
}

void MenuButton::setIsSelected(bool t_isSelected)
{
	m_isSelected = t_isSelected;
}

bool MenuButton::getIsSelected()
{
	return m_isSelected;
}

void MenuButton::render(SDL_Renderer* t_renderer)
{
	SDL_Rect destRect = SDL_Rect{ m_position.x, m_position.y, m_size.x, m_size.y };
	SDL_RenderCopy(t_renderer, m_buttonTexture, NULL, &destRect);
}
