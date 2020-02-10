#include "stdafx.h"
#include "VisualComponent.h"

VisualComponent::VisualComponent(std::string t_path, SDL_Renderer* t_renderer) :
	Component(ComponentType::Visual)
{
	if (!loadFromFile(t_path, t_renderer))
	{
		throw std::invalid_argument("Could not load file");
	}
}

VisualComponent::~VisualComponent()
{
	free();
}

void VisualComponent::free()
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

bool VisualComponent::loadFromFile(std::string t_path, SDL_Renderer* t_renderer)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified t_path
	SDL_Surface* loadedSurface = IMG_Load(t_path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", t_path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(t_renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", t_path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			m_width = loadedSurface->w;
			m_height = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	m_texture = newTexture;
	return m_texture != NULL;
}