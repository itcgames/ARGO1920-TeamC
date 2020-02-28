#pragma once
#include "Component.h"
#include <SDL.h>

struct Colour
{
	Uint8 red = 0;
	Uint8 green = 0;
	Uint8 blue = 0;
	Uint8 alpha = 255;
};

class ColourComponent : public Component
{
public:
	ColourComponent();
	ColourComponent(Uint8 t_red, Uint8 t_green, Uint8 t_blue, Uint8 t_alpha);
	ColourComponent(Colour t_colour);
	~ColourComponent();

	//Set color
	void setColor(Uint8 t_red, Uint8 t_green, Uint8 t_blue, Uint8 t_alpha);
	void setColor(Colour t_colour);
	void setSecondaryColour(Colour t_colour);
	//Set individual colour
	void setRed(Uint8 t_red);
	void setGreen(Uint8 t_green);
	void setBlue(Uint8 t_blue);
	void setAlpha(Uint8 t_alpha);

	//get colour
	Colour getColour();
	Colour getSecondaryColour();
	//get individual colour
	Uint8 getRed();
	Uint8 getGreen();
	Uint8 getBlue();
	Uint8 getAlpha();

private:
	Colour m_colour;
	Colour m_secondaryColour;
};

