#include "stdafx.h"
#include "TextComponent.h"

/// <summary>
/// Default constructor to set up a text component. Takes a fileName, renderer, whether text is static and the text string
/// </summary>
/// <param name="t_fileName">file name of the font</param>
/// <param name="t_renderer">game renderer, needed to create the texture</param>
/// <param name="t_staticPos">whether the text is attached to the world or screen</param>
/// <param name="t_text">the text displayed</param>
TextComponent::TextComponent(std::string t_fileName, SDL_Renderer* t_renderer, bool t_staticPos, std::string t_text) :
	Component(ComponentType::Text),
	m_fontName(t_fileName),
	m_renderer(t_renderer),
	m_staticPosition(t_staticPos),
	m_pointSize(Utilities::MEDIUM_FONT),
	m_text(t_text)
{
	m_colour.r = 255;
	m_colour.g = 255;
	m_colour.b = 255;
	m_colour.a = 255;

	init();
}

TextComponent::TextComponent(std::string t_fileName, SDL_Renderer* t_renderer, int t_size, bool t_staticPos, std::string t_text, Uint8 t_red, Uint8 t_green, Uint8 t_blue, Uint8 t_alpha) :
	Component(ComponentType::Text),
	m_fontName(t_fileName),
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
	updateTexture();
}

TextComponent::~TextComponent()
{
	free();
}

void TextComponent::free()
{
	m_width = 0;
	m_height = 0;
	m_texture = NULL;
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

void TextComponent::setSize(int t_size)
{
	m_pointSize = t_size;
	m_texture = AssetManager::Instance()->GetText(m_text, m_fontName, m_pointSize, m_colour);
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

bool TextComponent::hasStaticPos() const
{
	return m_staticPosition;
}

SDL_Texture* TextComponent::getTexture() const
{
	return m_texture;
}

void TextComponent::updateTexture()
{
	m_texture = AssetManager::Instance()->GetText(m_text, m_fontName, m_pointSize, m_colour);
	SDL_QueryTexture(m_texture, NULL, NULL, &m_width, &m_height);
}
