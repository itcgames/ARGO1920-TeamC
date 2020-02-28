#include "stdafx.h"
#include "VisualComponent.h"

VisualComponent::VisualComponent(std::string t_filename, SDL_Renderer* t_renderer, glm::vec2 t_offset, Uint8 t_red, Uint8 t_green, Uint8 t_blue, bool t_staticPosition) :
	Component(ComponentType::Visual),
	m_staticPosition(t_staticPosition),
	m_offset(t_offset)
{
	if (!loadFromFile(t_filename, t_renderer))
	{
		throw std::invalid_argument("Could not load file");
	}
	setColor(t_red, t_green, t_blue);
}

VisualComponent::~VisualComponent()
{
	//no longer calling free() here as AssetManager handles freeing resources
	m_texture = NULL;
}

void VisualComponent::setColor(Uint8 t_red, Uint8 t_green, Uint8 t_blue)
{
	SDL_SetTextureColorMod(m_texture, t_red, t_green, t_blue);
}

void VisualComponent::setBlendMode(SDL_BlendMode t_blending)
{
	//Set t_blending function
	SDL_SetTextureBlendMode(m_texture, t_blending);
}

void VisualComponent::setAlpha(Uint8 t_alpha)
{
	//Modulate texture t_alpha
	SDL_SetTextureAlphaMod(m_texture, t_alpha);
}

int VisualComponent::getWidth() const
{
	return m_width;
}

int VisualComponent::getHeight() const
{
	return m_height;
}

SDL_Texture* VisualComponent::getTexture() const
{
	return m_texture;
}

void VisualComponent::setStaticPosition(bool t_staticPosition)
{
	m_staticPosition = t_staticPosition;
}

bool VisualComponent::getStaticPosition()
{
	return m_staticPosition;
}

glm::vec2 VisualComponent::getOffset()
{
	return m_offset;
}

void VisualComponent::setOffset(glm::vec2 t_offset)
{
	m_offset = t_offset;
}

bool VisualComponent::loadFromFile(std::string t_filename, SDL_Renderer* t_renderer)
{
	//Return success
	m_texture = AssetManager::Instance()->GetTexture(t_filename);

	if (m_texture != NULL)
	{
		SDL_QueryTexture(m_texture, NULL, NULL, &m_width, &m_height);
	}
	return m_texture != NULL;
}