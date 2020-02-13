#include "stdafx.h"
#include "Game.h"

/// <summary>
/// Constructor for the game class.
/// </summary>

class State;
Game::Game() :
	m_transformSystem{ m_eventManager },
	m_framesPerSecond(60),
	m_ticksPerSecond(60),
	m_lastTick(0),
	m_lastRender(0),
	m_timePerFrame(0),
	m_timePerTick(0)
{
	try
	{
		m_timePerFrame = 1000 / m_framesPerSecond;
		m_timePerTick = 1000 / m_ticksPerSecond;
		// Try to initalise SDL in general
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) throw "Error Loading SDL";

		// Create SDL Window Centred in Middle Of Screen
		m_window = SDL_CreateWindow("ARGO", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Utilities::SCREEN_WIDTH, Utilities::SCREEN_HEIGHT, NULL);
		// Check if window was created correctly
		if (!m_window) throw "Error Loading Window";

		//Create the SDL Renderer 
		m_renderer = SDL_CreateRenderer(m_window, -1, 0);
		//Check if the renderer was created correctly
		if (!m_renderer) throw "Error Loading Renderer";

		// Sets clear colour of renderer to black and the color of any primitives
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
		// Game is running
		m_isRunning = true;


		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		//Dark mode
		ImGui::StyleColorsDark();

		ImGui_ImplSDL2_InitForD3D(m_window);
		ImGuiSDL::Initialize(m_renderer, 800, 600);
		m_eventManager.subscribeToEvent<CloseWindow>(std::bind(&Game::closeWindow, this, std::placeholders::_1));

		std::map<ButtonType, Command*> buttonPressMap = {
			std::pair<ButtonType, Command*>(ButtonType::DpadUp, new MoveUpCommand()),
			std::pair<ButtonType, Command*>(ButtonType::DpadDown, new MoveDownCommand()),
			std::pair<ButtonType, Command*>(ButtonType::DpadLeft, new MoveLeftCommand()),
			std::pair<ButtonType, Command*>(ButtonType::DpadRight, new MoveRightCommand()),
			std::pair<ButtonType,Command*>(ButtonType::Back, new CloseWindowCommand()) };

		//add components to player
		for (auto& player : m_players)
		{
			player.addComponent(new HealthComponent(10, 10));
			player.addComponent(new TransformComponent());
			// passing two of the same object as at this moment the commands for button press is the same for button held
			player.addComponent(new InputComponent(buttonPressMap, buttonPressMap));
			player.addComponent(new ForceComponent());
			player.addComponent(new ColliderCircleComponent(Utilities::PLAYER_RADIUS));
			player.addComponent(new ColourComponent(glm::linearRand(0, 255), glm::linearRand(0, 255), glm::linearRand(0, 255), 255));
		}

		m_entities.reserve(MAX_ENTITIES);
		for (int i = 0; i < 5; i++)
		{
			m_entities.emplace_back();
			m_entities.at(i).addComponent(new TransformComponent());
			m_entities.at(i).addComponent(new AiComponent());
			m_entities.at(i).addComponent(new ColliderCircleComponent(Utilities::ENEMY_RADIUS));
		}

		setupLevel();
	}
	catch (std::string error)
	{
		// output error if any part above doesnt load correctly
		std::cout << error << std::endl;
		// game doesnt run
		m_isRunning = false;
	}

}

/// <summary>
/// destructor for the game class
/// </summary>
Game::~Game()
{
	cleanup();
}

/// <summary>
/// function for the main game loop
/// </summary>
void Game::run()
{
	m_lastTick = SDL_GetTicks();
	m_lastRender = SDL_GetTicks();
	while (m_isRunning)
	{
		Uint32 currentTick = SDL_GetTicks();
		Uint16 deltaTime = currentTick - m_lastTick;
		Uint16 renderTime = currentTick - m_lastRender;

		bool canRender = checkCanRender(renderTime);
		bool canTick = checkCanTick(deltaTime);
		processEvent();
		update(canRender, canTick, deltaTime);

		if (canRender) 
		{//Draw Here
			ImGui::Render();
			ImGuiSDL::Render(ImGui::GetDrawData());
			SDL_RenderPresent(m_renderer);
		}
	}
}

/// <summary>
/// Processes any SDL event
/// </summary>
void Game::processEvent()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	ImGui_ImplSDL2_ProcessEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		m_isRunning = false;
		break;
	case SDL_KEYDOWN:
		// Press Escape to close screen
		if (SDLK_ESCAPE == event.key.keysym.sym)
		{
			m_isRunning = false;
		}
		if (SDLK_SPACE == event.key.keysym.sym)
		{
			if (m_entities.size() > 0)
			{
				m_entities.erase(--m_entities.end());
			}
			std::cout << m_entities.size() << std::endl;
		}
		if (SDLK_BACKSPACE == event.key.keysym.sym)
		{
			//delete all entities
			if (m_entities.size() > 0)
			{
				m_entities.erase(m_entities.begin(), m_entities.end());
			}
			std::cout << m_entities.size() << std::endl;
		}
		if (SDLK_DELETE == event.key.keysym.sym)
		{
			//delete all entities
			m_players[0].removeCompType(ComponentType::Input);
		}
		if (SDLK_RETURN == event.key.keysym.sym)
		{
			//check if we can add 100 entities
			int availableSpace = MAX_ENTITIES - m_entities.size();
			//if more than 100 available, set to 100
			if (availableSpace > 100)
			{
				availableSpace = 100;
			}
			//if at least 1 available
			if (availableSpace > 0)
			{
				for (int i = 0; i < availableSpace; i++)
				{
					m_entities.emplace_back();
					m_entities.at(m_entities.size() - 1).addComponent(new TransformComponent());
					m_entities.at(m_entities.size() - 1).addComponent(new AiComponent());
					m_entities.at(m_entities.size() - 1).addComponent(new ColliderCircleComponent(Utilities::ENEMY_RADIUS));
				}
			}
			std::cout << m_entities.size() << std::endl;
		}
		if (SDLK_1 == event.key.keysym.sym)
		{
			//if space available in the vector
			if (m_entities.size() < MAX_ENTITIES)
			{
				//add one
				m_entities.emplace_back();
				m_entities.at(m_entities.size() - 1).addComponent(new TransformComponent());
				m_entities.at(m_entities.size() - 1).addComponent(new AiComponent());
			}
			std::cout << m_entities.size() << std::endl;
		}
		break;
	default:
		break;
	}
}

void Game::update(bool t_canTick, bool t_canRender, Uint16 t_dt)
{
	if (t_canRender)
	{
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();
	
	if (showDemoWindow)
	{
		ImGui::ShowDemoWindow(&showDemoWindow);
	}

		static float f = 0.0f;
		
		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &showDemoWindow);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &showanotherWindow);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		if (showanotherWindow)
		{
			ImGui::Begin("Another Window", &showanotherWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				showanotherWindow = false;
			ImGui::End();
		}
		//std::cout << counter << std::endl;
 

	//cstr = "";
	ImGui::Begin("Hello World");

	ImGuiHelper::InputText(windowTitle, "Window Title" );
	ImGui::SameLine();
	if (ImGui::Button("Set Title"))
	{
		SDL_SetWindowTitle(m_window, windowTitle.c_str());
	}	
	ImGui::InputFloat("Counter", &counter, 0.5f, 2.3f, ImGuiHelper::GetDecimalPercision(25).c_str());
	ImGuiHelper::InputFloat2(vec2, "InputFloat2");
	ImGuiHelper::InputFloat3(vec3, "inputFloat3");
	ImGui::End();

	//SDL_SetRenderDrawColor(m_renderer,clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	
		//SDL_RenderClear(m_renderer);

	
	}
	
	for (auto& entity : m_entities)
	{
		if (t_canTick)
		{
			m_inputSystem.update(entity, m_eventManager);
			m_hpSystem.update(entity);
			m_aiSystem.update(entity);
			m_transformSystem.update(entity);
			m_collisionSystem.update(entity);
		}
		if (t_canRender)
		{
			m_renderSystem.render(m_renderer, entity);
		}
	}
	for (auto& entity : m_levelTiles)
	{
		if (t_canTick)
		{
			m_inputSystem.update(entity, m_eventManager);
			m_hpSystem.update(entity);
			m_aiSystem.update(entity);
			m_transformSystem.update(entity);
			m_collisionSystem.update(entity);
		}
		if (t_canRender)
		{
			m_renderSystem.render(m_renderer, entity);
		}
	}
	for (auto& player : m_players)
	{
		if (t_canTick)
		{
			m_inputSystem.update(player, m_eventManager);
			m_hpSystem.update(player);
			m_aiSystem.update(player);
			m_transformSystem.update(player);
			m_collisionSystem.update(player);
		}
		if (t_canRender)
		{
			m_renderSystem.render(m_renderer, player);
		}
	}
	if (t_canTick) m_collisionSystem.handleCollisions();
}

void Game::preRender()
{
	//setting the focus point for the camera.
	glm::vec2 focusPoint = glm::vec2(0, 0);
	for (auto& player : m_players)
	{
		focusPoint.x += static_cast<TransformComponent*>(player.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos().x;
		focusPoint.y += static_cast<TransformComponent*>(player.getAllComps().at(COMPONENT_ID::TRANSFORM_ID))->getPos().y;
	}
	m_renderSystem.setFocus(focusPoint / 4.0f);


	SDL_RenderClear(m_renderer);
	
}

/// <summary>
/// Cleans up after running by deleting stuff
/// </summary>
void Game::cleanup()
{
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	SDL_QUIT;
}

void Game::setupLevel()
{
	int count = 0;
	m_levelTiles.reserve(Utilities::LEVEL_TILE_HEIGHT * Utilities::LEVEL_TILE_WIDTH);
	for (int i = 0; i < Utilities::LEVEL_TILE_HEIGHT; i++)
	{
		for (int j = 0; j < Utilities::LEVEL_TILE_WIDTH; j++)
		{
			m_levelTiles.emplace_back();
			m_levelTiles.at(count).addComponent(new TransformComponent(j * Utilities::TILE_SIZE, i * Utilities::TILE_SIZE, 0));
			m_levelTiles.at(count).addComponent(new VisualComponent("assets//images//Texture.png", m_renderer));
			m_levelTiles.at(count).addComponent(new ColliderAABBComponent(glm::vec2(Utilities::TILE_SIZE, Utilities::TILE_SIZE)));
			count++;
		}
	}
}

bool Game::checkCanRender(Uint16 t_renderTime)
{
	if (t_renderTime > m_timePerFrame)
	{
		m_lastRender += m_timePerFrame;
		preRender();
		return true;
	}
	return false;
}

bool Game::checkCanTick(Uint16 t_deltaTime)
{
	if (t_deltaTime > m_timePerTick)
	{
		m_lastTick += m_timePerTick;
		return true;
	}
	return false;
}

void Game::closeWindow(const CloseWindow& t_event)
{
	m_isRunning = false;
}
