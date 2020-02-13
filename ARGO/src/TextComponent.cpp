#include "stdafx.h"
#include "TextComponent.h"


TextComponent::TextComponent(TTF_Font* t_font, SDL_Renderer* t_renderer, bool t_staticPos, std::string t_text) :
	Component(ComponentType::Text),
	m_font(t_font),
	m_renderer(t_renderer),
	m_staticPosition(t_staticPos),
	m_text(t_text)
{
	m_pointSize = TTF_FontHeight(m_font);
	m_colour.r = 255;
	m_colour.g = 255;
	m_colour.b = 255;
	m_colour.a = 255;

	init();
}

TextComponent::TextComponent(TTF_Font* t_font, SDL_Renderer* t_renderer, int t_size, bool t_staticPos, std::string t_text, Uint8 t_red, Uint8 t_green, Uint8 t_blue, Uint8 t_alpha) :
	Component(ComponentType::Text),
	m_font(t_font),
	m_renderer(t_renderer),
	m_pointSize(t_size),
	m_staticPosition(t_staticPos),
	m_text(t_text)
{
	m_colour.r = t_red;
	m_colour.g = t_green;
	m_colour.b = t_blue;
	m_colour.a = t_alpha;

	init();
}

void TextComponent::init()
{
	if (TTF_FontHeight(m_font) != m_pointSize)
	{
		int currentSize = TTF_FontHeight(m_font);
		float ratio = (float)m_pointSize / (float)currentSize;

		TTF_SizeText(m_font, m_text.c_str(), &m_width, &m_height);
		m_width *= ratio;
		m_height *= ratio;
	}

	updateTexture();
}

TextComponent::~TextComponent()
{
	free();
}

void TextComponent::free()
{
	if (m_texture != NULL)
	{
		SDL_DestroyTexture(m_texture);
		m_texture = NULL;
	}
	m_width = 0;
	m_height = 0;
}

void TextComponent::setColor(Uint8 t_red, Uint8 t_green, Uint8 t_blue)
{
	m_colour.r = t_red;
	m_colour.g = t_green;
	m_colour.b = t_blue;

	updateTexture();
}

void TextComponent::setAlpha(Uint8 t_alpha)
{
	m_colour.a = t_alpha;
	updateTexture();
}

void TextComponent::setText(std::string t_text)
{
	m_text = t_text;
	updateTexture();
}

std::string TextComponent::getText() const
{
	return m_text;
}

int TextComponent::getWidth() const
{
	return m_width;
}

int TextComponent::getHeight() const
{
	return m_height;
}

SDL_Color TextComponent::getColour() const
{
	return m_colour;
}

bool TextComponent::hasStatisPos() const
{
	return m_staticPosition;
}

SDL_Texture* TextComponent::getTexture() const
{
	return m_texture;
}

void TextComponent::updateTexture()
{
	SDL_Surface* surface = NULL;

	surface = TTF_RenderText_Solid(m_font, m_text.c_str(), m_colour);

	m_texture = SDL_CreateTextureFromSurface(m_renderer, surface);

	if (m_height == 0 && m_width == 0)
	{
		SDL_QueryTexture(m_texture, NULL, NULL, &m_width, &m_height);
	}

	SDL_FreeSurface(surface);
}
