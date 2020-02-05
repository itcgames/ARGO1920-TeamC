#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

#include "Entity.h"
#include "HealthComponent.h"
#include "PositionComponent.h"
#include "InputComponent.h"
#include "HealthSystem.h"
#include "PositionSystem.h"
#include "InputSystem.h"
#include "RenderSystem.h"
#include "AiSystem.h"

class Game
{
public:

	Game();
	~Game();

	void run();

private:
	void processEvents();
	void update();
	void render();
	void cleanup();
	void handleInput(SDL_Event* t_event);
	// window used in the program
	SDL_Window* m_window;

	// used for drawing things onto the screen
	SDL_Renderer* m_renderer;

	// texture used for entities
	SDL_Texture* m_texture;

	// if game loop is happening
	bool isRunning;

	std::string getErrorString(std::string t_errorMsg);

	Entity* m_player;
	Entity* m_alien;
	Entity* m_dog;
	Entity* m_cat;

	//declare an entity that is a unique ptr
	std::vector<std::unique_ptr<Entity>> m_smartVector;


	const int PLAYER_ID = 0;
	const int ALIEN_ID = 1;
	const int DOG_ID = 2;
	const int CAT_ID = 3;

	HealthSystem m_hpSystem;
	PositionSystem m_posSystem;
	InputSystem m_inputSystem;
	RenderSystem m_renderSystem;
	AiSystem m_aiSystem;
};

