#pragma once
#include "Component.h"
#include <SDL.h>
#include "SDL_ttf.h"
#include <string>

class TextComponent : public Component
{
public:
	TextComponent(TTF_Font* t_font, SDL_Renderer* t_renderer, bool t_staticPos = true, std::string t_text = "Default Text");
	TextComponent(TTF_Font* t_font, SDL_Renderer* t_renderer, int t_size, bool t_staticPos = true, std::string t_text = "Default Text", Uint8 t_red = 255, Uint8 t_green = 255, Uint8 t_blue = 255, Uint8 t_alpha = 255);
	~TextComponent();

	void init();

	//Deallocates pointers
	void free();

	//Set color
	void setColor(Uint8 t_red, Uint8 t_green, Uint8 t_blue);

	//Set alpha modulation
	void setAlpha(Uint8 t_alpha);

	//Set text
	void setText(std::string t_text);

	//set x and y pos for text

	//Gets image dimensions
	int getWidth() const;
	int getHeight() const;
	SDL_Color getColour() const;
	bool hasStatisPos() const;

	//Get text incase its needed
	std::string getText() const;

	//get texture ptr
	SDL_Texture* getTexture() const;
	SDL_Surface* getSurface() const;

private:
	void updateSurface();

	//The actual hardware texture
	SDL_Texture* m_texture;
	SDL_Surface* m_surface;
	TTF_Font* m_font;
	SDL_Color m_colour;
	SDL_Renderer* m_renderer;

	//Text dimensions
	int m_width = 0;
	int m_height = 0;
	int m_pointSize;

	bool m_staticPosition;

	std::string m_text;
};

