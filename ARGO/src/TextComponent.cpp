#include "stdafx.h"
#include "TextComponent.h"

TextComponent::TextComponent(std::string t_fontPath) :
	Component(ComponentType::Text)
{
	if (TTF_Init() == -1)
	{
		printf("TTF_Init: %s\n", TTF_GetError());
	}

	m_font = TTF_OpenFont(t_fontPath.c_str(), 24);
	if (!m_font)
	{
		std::cout << "Failed to load font" << std::endl;
	}
}

TextComponent::~TextComponent()
{
	free();
}

bool TextComponent::loadFromRenderedText(std::string t_textureText, SDL_Color t_textColour, SDL_Renderer* t_renderer)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, t_textureText.c_str(), t_textColour);
	if (textSurface != NULL)
	{
		//Create texture from surface pixels
		m_texture = SDL_CreateTextureFromSurface(t_renderer, textSurface);
		if (m_texture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			m_width = textSurface->w;
			m_height = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}

	//Return success
	return m_texture != NULL;
}

void TextComponent::free()
{
	//Free texture if it exists
	if (m_texture != NULL)
	{
		SDL_DestroyTexture(m_texture);
		m_texture = NULL;
		m_width = 0;
		m_height = 0;
	}
}

void TextComponent::setColor(Uint8 t_red, Uint8 t_green, Uint8 t_blue)
{
	SDL_SetTextureColorMod(m_texture, t_red, t_green, t_blue);
}

void TextComponent::setBlendMode(SDL_BlendMode t_blending)
{
	//Set t_blending function
	SDL_SetTextureBlendMode(m_texture, t_blending);
}

void TextComponent::setAlpha(Uint8 t_alpha)
{
	//Modulate texture t_alpha
	SDL_SetTextureAlphaMod(m_texture, t_alpha);
}

int TextComponent::getWidth() const
{
	return m_width;
}

int TextComponent::getHeight() const
{
	return m_height;
}

SDL_Texture* TextComponent::getTexture() const
{
	return m_texture;
}
