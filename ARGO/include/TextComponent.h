#pragma once
#include "Component.h"
#include <SDL.h>
#include <SDL_image.h>
#include "SDL_ttf.h"
#include <string>

class TextComponent : public Component
{
public:
	TextComponent(std::string t_fontPath);
	~TextComponent();

	bool loadFromRenderedText(std::string t_textureText, SDL_Color t_textColour, SDL_Renderer* t_renderer);
	TTF_Font* m_font;

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 t_red, Uint8 t_green, Uint8 t_blue);

	//Set blend mode
	void setBlendMode(SDL_BlendMode t_blending);

	//Set t_alpha modulation
	void setAlpha(Uint8 t_alpha);

	//Gets image dimensions
	int getWidth() const;
	int getHeight() const;

	//get texture ptr
	SDL_Texture* getTexture() const;

private:
	//The actual hardware texture
	SDL_Texture* m_texture;
	SDL_Surface* m_surface;

	TTF_Font* m_font;

	//Image dimensions
	int m_width;
	int m_height;
};

