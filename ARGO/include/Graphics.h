#pragma once
#include <SDL.h>
#include <stdio.h>
#include "Utilities.h"

class Graphics
{
public:
	static const int SCREEN_WIDTH = Utilities::SCREEN_WIDTH;
	static const int SCREEN_HEIGHT= Utilities::SCREEN_HEIGHT;

private:
	static Graphics* s_instance;
	static bool s_initilised;

	SDL_Window* m_window;
	SDL_Surface* m_backBuffer;

public:
	static Graphics* instance();
	static void release();
	static bool initialised();

	void render();
private:
	Graphics();
	~Graphics();

	bool init();

};

