#pragma once
#include "Component.h"
#include <SDL.h>
#include <SDL_image.h>
#include "SDL_ttf.h"
#include <string>

class VisualComponent : public Component
{
public:
	VisualComponent(std::string t_path, SDL_Renderer* t_renderer);
	~VisualComponent();

	//Loads image at specified t_path
	bool loadFromFile(std::string t_path, SDL_Renderer* t_renderer);

#ifdef _SDL_TTF_H
	//Creates image from font string
	VisualComponent(std::string t_fontPath);
	bool loadFromRenderedText(std::string t_textureText, SDL_Color t_textColour, SDL_Renderer* t_renderer);
	TTF_Font* m_font;
#endif

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

	//Image dimensions
	int m_width;
	int m_height;
};

