#include "stdafx.h"
#include "Graphics.h"

Graphics* Graphics::s_instance = NULL;
bool Graphics::s_initilised = false;

Graphics* Graphics::instance()
{
	if (s_instance == NULL)
	{
		s_instance = new Graphics();
	}
	return s_instance;
}

void Graphics::release()
{
	delete s_instance;
	s_instance = NULL;
	s_initilised = false;
}

bool Graphics::initialised()
{
	return s_initilised;
}

void Graphics::render()
{
	SDL_UpdateWindowSurface(m_window);
}

Graphics::Graphics()
{
	m_backBuffer = NULL;

	s_initilised = init();
}

Graphics::~Graphics()
{
	SDL_DestroyWindow(m_window);
	m_window == NULL;

	SDL_Quit();
}

bool Graphics::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL initialization error: %s\n", SDL_GetError());
	}
	m_window = SDL_CreateWindow("ARGO", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (m_window == NULL)
	{
		printf("Window creation error: %s\n", SDL_GetError());
		return false;
	}
	m_backBuffer = SDL_GetWindowSurface(m_window);

	return true;
}
